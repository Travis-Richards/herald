#pragma once

#include <QStackedWidget>

class ControlPanel;
class GameList;
class PanicWidget;

/// The central widget is simply the
/// widget that goes inside the main window.
/// It contains the control panel used to
/// choose the games to play.
class CentralWidget : public QStackedWidget {
  Q_OBJECT
public:
  /// Constructs the cetral widget.
  /// @param parent A pointer to the parent widget.
  /// This should be a pointer to the main window.
  CentralWidget(QWidget* parent);
  /// Displays the panic widget with a specified message.
  /// @param message The message to be displayed. This can
  /// include formatting for rich text attributes.
  void panic(const QString& message);
  /// Updates the game list view.
  /// This happens at startup and when the user opens a new game.
  void update_game_list(const GameList& game_list);
signals:
  /// Emitted when the "open" button
  /// is clicked from the control panel.
  void open_requested();
  /// Emitted when the "play" button
  /// is clicked from the control panel.
  void play_requested();
  /// Emitted when a game is selected
  /// from the control panel.
  void game_selected(int index);
private:
  /// A game control panel.
  ControlPanel* control_panel;
  /// A pointer to the panic widget.
  /// This gets displayed if a fatal errors occurs.
  PanicWidget* panic_widget;
};
