#include "GameListView.h"

#include "GameInfo.h"
#include "GameList.h"

#include <QHeaderView>
#include <QStringList>

GameListView::GameListView(QWidget* parent) : QTableWidget(parent) {

  // Setup horizontal header
  QStringList labels;
  labels << tr("Title");
  labels << tr("Author");
  labels << tr("Location");
  setColumnCount(labels.size());
  setHorizontalHeaderLabels(labels);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // Disable vertical header
  verticalHeader()->hide();
  verticalHeader()->setSectionsClickable(true);

  setShowGrid(false);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setSelectionBehavior(QAbstractItemView::SelectRows);

  connect(this, &QTableWidget::clicked, this, &GameListView::on_game_clicked);
  connect(this, &QTableWidget::doubleClicked, this, &GameListView::on_game_double_clicked);
}

void GameListView::update(const GameList& game_list) {

  auto count = game_list.size();

  setRowCount(count);

  for (int i = 0; i < count; i++) {

    auto path = game_list.at(i);

    auto* info = GameInfo::open(path, this);
    if (info) {
      setItem(i, 0, new QTableWidgetItem(info->get_title()));
      setItem(i, 1, new QTableWidgetItem(info->get_author()));
      delete info;
    }

    setItem(i, 2, new QTableWidgetItem(path));
  }
}

void GameListView::on_game_clicked(const QModelIndex& index) {
  emit game_selected(index.row());
}

void GameListView::on_game_double_clicked(const QModelIndex& index) {
  emit game_play_requested(index.row());
}
