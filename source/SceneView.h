#pragma once

#include <QGraphicsView>

class Scene;
class QEvent;
class QSize;

/// A view of the game scene.
/// This is hidden until a game is started.
class SceneView : public QGraphicsView {
  Q_OBJECT
public:
  /// Constructs the scene view.
  /// @param scene The scene to be viewed.
  SceneView(Scene* scene);
signals:
  /// This signal is emitted right
  /// before the window for the scene
  /// view is going to get closed.
  void closing();
  /// This signal is emitted when the window is
  /// resized by the user.
  /// @param size The new size of the window.
  void resized(const QSize& size);
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
