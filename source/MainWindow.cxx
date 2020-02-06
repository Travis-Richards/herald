#include "MainWindow.h"

#include "CentralWidget.h"

#ifndef NO_CONFIG
#include "Config.h"
#endif

#include <QAction>
#include <QFileDialog>

MainWindow::MainWindow() {

  central_widget = new CentralWidget(this);

  setCentralWidget(central_widget);

  QString title("Herald");

#ifdef HERALD_VERSION
  title += QString(" " HERALD_VERSION);
#endif

  setWindowTitle(title);

  connect(central_widget, &CentralWidget::open_requested,   this, &MainWindow::open_from_dialog);
  connect(central_widget, &CentralWidget::play_requested,   this, &MainWindow::play_requested);
  connect(central_widget, &CentralWidget::delete_requested, this, &MainWindow::delete_requested);
  connect(central_widget, &CentralWidget::game_selected,    this, &MainWindow::game_selected);
}

void MainWindow::update_game_list(const GameList& game_list) {
  central_widget->update_game_list(game_list);
}

void MainWindow::open_from_dialog() {

  auto path = QFileDialog::getExistingDirectory(this, tr("Open an Existing Game"));
  if (path.isEmpty()) {
    return;
  }

  emit open_requested(path);
}

void MainWindow::closeEvent(QCloseEvent* close_event) {
  emit exit_requested();
  QMainWindow::closeEvent(close_event);
}
