#pragma once

#include <QTableWidget>

class GameList;
class QModelIndex;

/// This is a view of the available games to play.
class GameListView : public QTableWidget {
  Q_OBJECT
public:
  /// Constructs the game list view.
  GameListView(QWidget* parent);
  /// Updates the list of games to display.
  void update(const GameList& game_list);
signals:
  /// Emitted when a game is selected from the list.
  /// @param index The index of the game within the game list.
  void game_selected(int index);
  /// Emitted when a game on the list is double clicked.
  /// This indicates that the user would like to select and play the game.
  /// @param index The index of the game within the game list.
  void game_play_requested(int index);
protected slots:
  /// This function is only here to convert
  /// the model index to a single integer index
  /// before the @ref GameListView::game_selected
  /// signal is emitted.
  void on_game_clicked(const QModelIndex& index);
  /// This function converts the model index to a regular
  /// game index and emits the @ref GameListView::game_play_requested signal.
  void on_game_double_clicked(const QModelIndex& index);
};
