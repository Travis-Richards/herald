#include <QApplication>

#include "Manager.h"
#include "MainWindow.h"

int main(int argc, char** argv) {

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Taylor Holberton");
  QCoreApplication::setApplicationName("Herald");

  Q_INIT_RESOURCE(icons);

  auto* manager = Manager::make(&app);

  MainWindow main_window;
  main_window.resize(640, 480);
  main_window.show();

  QObject::connect(manager, &Manager::updated_game_list, &main_window, &MainWindow::update_game_list);

  QObject::connect(&main_window, &MainWindow::open_requested,   manager, &Manager::open_game);
  QObject::connect(&main_window, &MainWindow::play_requested,   manager, &Manager::play_selected_game);
  QObject::connect(&main_window, &MainWindow::exit_requested,   manager, &Manager::handle_exit);
  QObject::connect(&main_window, &MainWindow::delete_requested, manager, &Manager::delete_game);
  QObject::connect(&main_window, &MainWindow::game_selected,    manager, &Manager::select_game);

  manager->load_settings();

  return app.exec();
}
