#pragma once

class QWidget;
class QGraphicsView;

namespace herald {

template <typename T>
class ScopedPtr;

class Controller;
class QtModel;

/// A target for the Qt engine to render to.
class QtTarget {
public:
  /// Creates a new Qt target instance.
  /// @param parent A pointer to the parent widget.
  /// @returns A new Qt target instance.
  static ScopedPtr<QtTarget> make(QWidget* parent);
  /// Just a stub.
  virtual ~QtTarget() {}
  /// Connects a model to the event system.
  /// This allows the model to be scaled to the size of the window view.
  /// This function is only here to be called by @ref QtEngine.
  virtual void connect_model(QtModel* model) = 0;
  /// Accesses a pointer to the graphics view.
  /// @returns The pointer to the graphics view.
  virtual QGraphicsView* get_graphics_view() = 0;
  /// Accesses a pointer to the window controller.
  /// @returns A pointer to the window controller.
  virtual Controller* get_controller() = 0;
  /// Shows the target window.
  virtual void show() = 0;
  /// Hides the target window.
  virtual void hide() = 0;
};

} // namespace herald
