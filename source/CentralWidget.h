#pragma once

#include <QWidget>

class ControlPanel;
class GameList;
class QLayout;

/// The central widget is simply the
/// widget that goes inside the main window.
/// It contains the control panel used to
/// choose the games to play.
class CentralWidget : public QWidget {
  Q_OBJECT
public:
  /// Constructs the cetral widget.
  /// @param parent A pointer to the parent widget.
  /// This should be a pointer to the main window.
  CentralWidget(QWidget* parent);
  /// Updates the game list view.
  /// This happens at startup and when the user opens a new game.
  void update_game_list(const GameList& game_list);
signals:
  /// Emitted when the "delete" button
  /// is clicked from the control panel.
  void delete_requested();
  /// Emitted when a game is selected
  /// from the control panel.
  void game_selected(int index);
  /// Emitted when the "open" button
  /// is clicked from the control panel.
  void open_requested();
  /// Emitted when the "play" button
  /// is clicked from the control panel.
  void play_requested();
  /// Emitted when the "settings" button
  /// is clicked from the control panel.
  void settings_requested();
private:
  /// A game control panel.
  ControlPanel* control_panel;
  /// The layout of the widget.
  QLayout* layout;
};
