#pragma once

#include <QWidget>

class GameList;
class GameListView;
class QGridLayout;
class QPushButton;

/// The control panel is the widget which contains
/// the game list view and can select, play, pause,
/// and save games.
class ControlPanel : public QWidget {
  Q_OBJECT
public:
  /// Constructs the control panel.
  /// @param parent A pointer to the parent widget.
  ControlPanel(QWidget* parent);
  /// Updates the list of available games to play.
  void update_game_list(const GameList& game_list);
signals:
  /// Emitted when the "open" button is clicked.
  void open_requested();
  /// Emitted when the "refresh" button is clicked.
  void refresh_requested();
  /// Emitted when the "play" button is clicked.
  void play_requested();
  /// Emitted when the "pause" button is clicked.
  void pause_requested();
  /// Emitted when the "save" button is clicked.
  void save_requested();
  /// Emitted when the "settings" button is clicked.
  void settings_requested();
  /// Emitted when the "delete" button is clicked.
  void delete_requested();
  /// Emitted when a game is clicked in the game list view.
  void game_selected(int index);
protected slots:
  /// This handles a double click action on
  /// a game from the game list. It converts
  /// the game play request signal to a game select
  /// and a play signal.
  void on_game_play_request(int index);
private:
  /// Opens a new game.
  QPushButton* open_button;
  /// Refreshes the game list.
  QPushButton* refresh_button;
  /// Starts or unpauses the game.
  QPushButton* play_button;
  /// Pauses the game.
  QPushButton* pause_button;
  /// Saves the game state.
  QPushButton* save_button;
  /// Opens the game engine settings.
  QPushButton* settings_button;
  /// Deletes a game from the game list.
  QPushButton* trash_button;
  /// A view of the available games to play.
  GameListView* game_list_view;
  /// The layout of the control panel.
  QGridLayout* layout;
};
