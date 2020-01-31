#pragma once

#include <QGraphicsView>

class Scene;
class QCloseEvent;

/// A view of the game scene.
/// This is hidden until a game is started.
class SceneView : public QGraphicsView {
  Q_OBJECT
public:
  /// Constructs the scene view.
  /// @param scene The scene to be viewed.
  /// @param parent A pointer to the parent widget.
  /// Normally, this should be null.
  SceneView(Scene* scene, QWidget* parent = nullptr);
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
  /// Overrides the close event so that a signal
  /// can be emitted and the API can be released
  /// for the window.
  void closeEvent(QCloseEvent* close_event) override;
};
