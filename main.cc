#include <QApplication>

#include "Engine.h"
#include "MainWindow.h"
#include "Scene.h"
#include "SceneView.h"

int main(int argc, char** argv) {

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Taylor Holberton");
  QCoreApplication::setApplicationName("Herald");

  Scene scene;

  SceneView scene_view(&scene);

  Engine engine(&scene);

  MainWindow main_window;
  main_window.resize(640, 480);
  main_window.show();

  QObject::connect(&engine, &Engine::panic, &main_window, &MainWindow::handle_engine_panic);
  QObject::connect(&engine, &Engine::updated_game_list, &main_window, &MainWindow::update_game_list);

  QObject::connect(&engine, &Engine::game_started, &scene_view, &SceneView::on_game_start);

  QObject::connect(&main_window, &MainWindow::open_requested, &engine, &Engine::open_game);
  QObject::connect(&main_window, &MainWindow::play_requested, &engine, &Engine::play_game);
  QObject::connect(&main_window, &MainWindow::exit_requested, &engine, &Engine::handle_exit);
  QObject::connect(&main_window, &MainWindow::game_selected, &engine, &Engine::select_game);

  engine.load_settings();

  return app.exec();
}
