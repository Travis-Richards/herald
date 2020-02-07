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
  /// Creates a new scene view instance.
  /// @param scene A pointer to the scene being rendered.
  /// @returns A new scene view instance.
  static SceneView* make(Scene* scene);
  /// Constructs the base scene view.
  /// @param scene The scene to be rendered.
  SceneView(Scene* scene);
  /// Just a stub.
  virtual ~SceneView() {}
signals:
  /// This signal is emitted right
  /// before the window for the scene
  /// view is going to get closed.
  void closing();
  /// This signal is emitted when the window is
  /// resized by the user.
  /// @param size The new size of the window.
  void resized(const QSize& size);
};
