#include "SceneView.h"

#include "Scene.h"

#include <QResizeEvent>

SceneView::SceneView(Scene* scene) : QGraphicsView(scene) {
  scene->set_view_size(size());
}

bool SceneView::event(QEvent* event) {

  if (event->type() == QEvent::Close) {
    emit closing();
  }

  return QGraphicsView::event(event);
}

void SceneView::resizeEvent(QResizeEvent* resize_event) {

  emit resized(resize_event->size());

  QGraphicsView::resizeEvent(resize_event);
}
