#pragma once

#include <QObject>

class Api;
class GameList;
class Scene;
class QSettings;
class QString;

enum class SymbolType : int;

/// This is the top level controller of the application
/// and (therefore) the game engine.
class Engine : public QObject {
  Q_OBJECT
public:
  /// Constructs the engine instance.
  /// @param scene A pointer to the game scene.
  /// This is (eventually) used when a game is started.
  /// @param parent A pointer to the parent object.
  Engine(Scene* scene, QObject* parent = nullptr);
  /// Releases resources allocated by the engine.
  ~Engine();
  /// Initializes the game object.
  /// @returns True on success, false on failure.
  bool init_game();
  /// Loads the engine settings.
  /// @returns True on success, false on failure.
  bool load_settings();
  /// Stores the engine settings.
  /// @returns True on success, false on failure.
  bool store_settings();
  /// Loads a specified level using the API.
  /// @param id The ID of the level to load.
  /// @param scene A pointer to the scene to put the data into.
  /// @returns True on success, false on failure.
  bool load_level(int id, Scene* scene);
  /// Accesses the number of levels in the loaded game.
  int get_level_count();
signals:
  /// This signal is emitted when a game
  /// is successfully opened so that the
  /// window for the game can be opened.
  /// @param title The title of the game being started.
  void game_started(const QString& title);
  /// This signal is emitted when the game
  /// is closing so that the window can lose
  /// visibility.
  void game_closed();
public slots:
  /// Opens a game with a specified directory path.
  /// @param path The path of the game to open.
  void open_game(const QString& path);
  /// Closes the API running the game.
  void close_game();
  /// Starts the selected game.
  /// If no game is selected, then nothing is done.
  void play_game();
  /// Handles operations required to execute
  /// before closing the application.
  void handle_exit();
  /// Selects a game from the game list.
  /// This is only required by the "play" button.
  void select_game(int index);
signals:
  /// Prints a panic message.
  /// This should only be used if the game
  /// completely fails to load a critical resource.
  /// @param msg The message to show the user.
  void panic(const QString& msg);
  /// Emitted when the game list is updated.
  void updated_game_list(const GameList& game_list);
protected slots:
  /// Handles the case of a symbol from a game instance.
  /// It is useful to show this information for game developers.
  /// @param type The type of symbol that was missing.
  /// @param name The name of the symbol that was missing.
  void handle_missing_symbol(SymbolType type, const char* name);
protected:
  /// Creates a Java game API instance.
  /// @param path The path to the game to start.
  /// @returns A new API instance for Java games.
  Api* make_java_api(const QString& path);
private:
  /// A pointer to the API used
  /// to control the scene.
  Api* api;
  /// A list of the available games.
  GameList* game_list;
  /// A pointer to the game scene.
  Scene* scene;
  /// The last selected game.
  int last_selected;
};
