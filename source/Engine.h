#pragma once

#include <QObject>

class GameList;
class QSettings;
class QString;

enum class SymbolType : int;

/// This is the top level controller of the application
/// and (therefore) the game engine.
class Engine : public QObject {
  Q_OBJECT
public:
  /// Creates a new engine instance.
  /// @param parent A pointer to the parent object.
  /// @returns A new engine instance.
  static Engine* make(QObject* parent);
  /// Constructs the engine instance.
  /// @param parent A pointer to the parent object.
  Engine(QObject* parent) : QObject(parent) {}
  /// Releases resources allocated by the engine.
  virtual ~Engine() {}
  /// Loads the engine settings.
  /// @returns True on success, false on failure.
  virtual bool load_settings() = 0;
  /// Saves the engine settings.
  /// @returns True on success, false on failure.
  virtual bool save_settings() const = 0;
public slots:
  /// Deletes the last selected game.
  virtual void delete_game() = 0;
  /// Opens a game with a specified directory path.
  /// @param path The path of the game to open.
  virtual void open_game(const QString& path) = 0;
  /// Starts the selected game.
  /// If no game is selected, then nothing is done.
  /// @returns True on success, false on failure.
  virtual bool play_selected_game() = 0;
  /// Handles operations required to execute
  /// before closing the application.
  virtual void handle_exit() = 0;
  /// Selects a game from the game list.
  /// This is only required by the "play" button.
  virtual void select_game(int index) = 0;
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
  /// Emitted when the game list is updated.
  void updated_game_list(const GameList& game_list);
};
