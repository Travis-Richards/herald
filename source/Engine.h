#pragma once

#include <QObject>

#include "SelectionIndex.h"

class ActiveGameList;
class GameList;
class QSettings;
class QString;

enum class SymbolType : int;

/// This is the top level controller of the application
/// and (therefore) the game engine.
class Engine : public QObject {
  Q_OBJECT
public:
  /// Constructs the engine instance.
  /// @param parent A pointer to the parent object.
  Engine(QObject* parent = nullptr);
  /// Releases resources allocated by the engine.
  ~Engine();
  /// Loads the engine settings.
  /// @returns True on success, false on failure.
  bool load_settings();
  /// Stores the engine settings.
  /// @returns True on success, false on failure.
  bool store_settings();
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
  /// Deletes the last selected game.
  void delete_game();
  /// Opens a game with a specified directory path.
  /// @param path The path of the game to open.
  void open_game(const QString& path);
  /// Closes the API running the game.
  void close_game();
  /// Starts the selected game.
  /// If no game is selected, then nothing is done.
  /// @returns True on success, false on failure.
  bool play_selected_game();
  /// Plays a game with a specified path.
  /// @param path The path to the game to play.
  /// @returns True on success, false on failure.
  bool play_game(const QString& path);
  /// Handles operations required to execute
  /// before closing the application.
  void handle_exit();
  /// Selects a game from the game list.
  /// This is only required by the "play" button.
  void select_game(int index);
signals:
  /// Emitted when the game list is updated.
  void updated_game_list(const GameList& game_list);
private:
  /// A pointer to a list containing the actively running games.
  ActiveGameList* active_games;
  /// A list of the available games.
  GameList* game_list;
  /// The last selected game.
  SelectionIndex last_selected;
};
