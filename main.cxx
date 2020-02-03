#include <QApplication>

#include "Engine.h"
#include "MainWindow.h"
#include "Scene.h"
#include "SceneView.h"

#include <QScreen>

int main(int argc, char** argv) {

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Taylor Holberton");
  QCoreApplication::setApplicationName("Herald");

  Scene scene;

  SceneView scene_view(&scene);

  auto* scene_view_widget = QWidget::createWindowContainer(&scene_view);
  scene_view_widget->setMinimumSize(QSize(320, 240));
  scene_view_widget->setMaximumSize(scene_view.screen()->size());

  Engine engine(&scene);

  MainWindow main_window;
  main_window.resize(640, 480);
  main_window.show();

  QObject::connect(&engine, &Engine::updated_game_list, &main_window, &MainWindow::update_game_list);

  //QObject::connect(&engine, &Engine::game_started, &scene_view, &SceneView::on_game_start);

  auto start_game = [scene_view_widget, &scene_view](const QString& title) {
    scene_view.show();
    scene_view_widget->setWindowTitle(title);
    scene_view_widget->show();
  };

  QObject::connect(&engine, &Engine::game_started, start_game);

  QObject::connect(&main_window, &MainWindow::open_requested, &engine, &Engine::open_game);
  QObject::connect(&main_window, &MainWindow::play_requested, &engine, &Engine::play_game);
  QObject::connect(&main_window, &MainWindow::exit_requested, &engine, &Engine::handle_exit);
  QObject::connect(&main_window, &MainWindow::game_selected, &engine, &Engine::select_game);

  engine.load_settings();

  return app.exec();
}
