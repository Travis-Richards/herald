#include "SceneView.h"

#include "KeyController.h"
#include "Scene.h"

#include <QResizeEvent>

namespace {

/// The full implementation of the scene view interface.
class SceneViewImpl final : public SceneView {
public:
  /// Constructs an instance of the scene view implementation.
  /// @param scene A pointer to the scene being rendered.
  SceneViewImpl(Scene* scene) : SceneView(scene) {
    key_controller = new KeyController(this);
  }
  /// Gets a pointer to the scene view's controller.
  Controller* get_controller() override {
    return key_controller;
  }
protected:
  /// Overrides the event handler so that a signal
  /// can be emitted and the API can be released
  /// for the window.
  bool event(QEvent* event) override;
  /// Overrides the resize event handler so that
  /// a signal can be emitted indicating that the
  /// window size has changed.
  void resizeEvent(QResizeEvent* resize_event) override;
  /// Handles a key press event.
  /// If a window based controller is being
  /// used, then it is forwarded to the controller.
  void keyPressEvent(QKeyEvent* key_event) override;
  /// Handles a key release event.
  /// If a window based controller is being
  /// used, then it is forward to the controller.
  void keyReleaseEvent(QKeyEvent* key_event) override;
private:
  /// A window key controller instance.
  KeyController* key_controller;
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

void SceneViewImpl::keyPressEvent(QKeyEvent* key_event) {
  key_controller->handle_key_press(key_event);
}

void SceneViewImpl::keyReleaseEvent(QKeyEvent* key_event) {
  key_controller->handle_key_release(key_event);
}

} // namespace

SceneView* SceneView::make(Scene* scene) {
  return new SceneViewImpl(scene);
}

SceneView::SceneView(Scene* scene) : QGraphicsView(scene) {
  setMinimumSize(QSize(640, 480));
  setRenderHints(QPainter::Antialiasing);
}
