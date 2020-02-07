#include "SceneView.h"

#include "Scene.h"

#include <QResizeEvent>

namespace {

/// The full implementation of the scene view interface.
class SceneViewImpl final : public SceneView {
public:
  /// Constructs an instance of the scene view implementation.
  /// @param scene A pointer to the scene being rendered.
  SceneViewImpl(Scene* scene) : SceneView(scene) {}
protected:
  /// Overrides the event handler so that a signal
  /// can be emitted and the API can be released
  /// for the window.
  bool event(QEvent* event) override;
  /// Overrides the resize event handler so that
  /// a signal can be emitted indicating that the
  /// window size has changed.
  void resizeEvent(QResizeEvent* resize_event) override;
};

bool SceneViewImpl::event(QEvent* event) {

  if (event->type() == QEvent::Close) {
    emit closing();
  }

  return SceneView::event(event);
}

void SceneViewImpl::resizeEvent(QResizeEvent* resize_event) {

  emit resized(resize_event->size());

  SceneView::resizeEvent(resize_event);
}

} // namespace

SceneView* SceneView::make(Scene* scene) {
  return new SceneViewImpl(scene);
}

SceneView::SceneView(Scene* scene) : QGraphicsView(scene) {
  setMinimumSize(QSize(640, 480));
  setRenderHints(QPainter::Antialiasing);
}
