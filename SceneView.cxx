#include "SceneView.h"

#include "Scene.h"

SceneView::SceneView(Scene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
  resize(640, 480);
}

void SceneView::on_game_start(const QString& title) {
  setWindowTitle(title);
  show();
}

void SceneView::closeEvent(QCloseEvent* event) {
  emit window_closing();
  QGraphicsView::closeEvent(event);
}
