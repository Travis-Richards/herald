#include "SceneView.h"

#include "Scene.h"

#include <QResizeEvent>

SceneView::SceneView(Scene* scene, QScreen* screen) : Qt3DExtras::Qt3DWindow(screen) {

  setRootEntity(scene->get_root_entity());

  scene->set_view_size(size());
}

bool SceneView::event(QEvent* event) {

  if (event->type() == QEvent::Close) {
    emit closing();
  }

  return Qt3DExtras::Qt3DWindow::event(event);
}

void SceneView::resizeEvent(QResizeEvent* resize_event) {

  emit resized(resize_event->size());

  Qt3DExtras::Qt3DWindow::resizeEvent(resize_event);
}
