#include "SceneView.h"

#include "Scene.h"

SceneView::SceneView(Scene* scene, QScreen* screen) : Qt3DExtras::Qt3DWindow(screen) {
  setRootEntity(scene->get_root_entity());
}

bool SceneView::event(QEvent* event) {

  if (event->type() == QEvent::Close) {
    emit window_closing();
  }

  return Qt3DExtras::Qt3DWindow::event(event);
}
