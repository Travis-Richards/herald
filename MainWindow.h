#pragma once

#include <QMainWindow>

class CentralWidget;
class GameList;
class QAction;
class QMenu;

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /// Constructs the main window.
  MainWindow();
signals:
  /// Emitted when a game is selected from the game list.
  /// @param index The index of the game within the game list.
  void game_selected(int index);
  /// Emitted when a game is requested to be opened.
  /// @param path The path of the game requesting to be opened.
  void open_requested(const QString& path);
  /// An exit of the application is being requested by the user.
  void exit_requested();
  /// Emitted when the user wants to play a certain game.
  void play_requested();
public slots:
  /// Handles the case that the game engine has a panic.
  /// @param msg The message describing what happened.
  void handle_engine_panic(const QString& msg);
  /// Updates the game in the game list view.
  /// This should be connected to the signal from @ref Engine
  /// that updates the game list either after a dialog window
  /// or on startup.
  void update_game_list(const GameList& game_list);
protected slots:
  /// Opens a game specified from a dialog window.
  void open_from_dialog();
protected:
  /// Overrides the close event for the main window.
  /// This is so that the engine can be notified that
  /// the application is getting closed down.
  void closeEvent(QCloseEvent* close_event) override;
private:
  /// The widget that gets displayed by
  /// the main window. This widget switches
  /// back and forth from a graphics view to
  /// a game list view.
  CentralWidget* central_widget;
  /// Contains buttons for adding games.
  QMenu* file_menu;
  /// Opens a new game.
  QAction* open_action;
};
