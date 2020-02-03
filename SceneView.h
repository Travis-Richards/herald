#pragma once

#include <Qt3DExtras/Qt3DWindow>

class Scene;
class QEvent;

/// A view of the game scene.
/// This is hidden until a game is started.
class SceneView : public Qt3DExtras::Qt3DWindow {
  Q_OBJECT
public:
  /// Constructs the scene view.
  /// @param scene The scene to be viewed.
  /// @param screen The screen to place the window on.
  SceneView(Scene* scene, QScreen* screen = nullptr);
signals:
  /// This signal is emitted right
  /// before the window for the scene
  /// view is going to get closed.
  void window_closing();
public slots:
  /// Handles the starting of a game.
  /// @param title The title of the game being started.
  /// This title is assigned to the window name.
  void on_game_start(const QString& title);
protected:
  /// Overrides the event handler so that a signal
  /// can be emitted and the API can be released
  /// for the window.
  bool event(QEvent* event) override;
};
