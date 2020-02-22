#include <herald/QtTarget.h>

#include <herald/ScopedPtr.h>

#include "QtKeyController.h"
#include "QtModel.h"

#include <QGraphicsView>
#include <QResizeEvent>

#include <vector>

namespace herald {

namespace {

/// A derived graphics view to setup the coordinate system.
class GraphicsView final : public QGraphicsView {
  /// The models connected to the graphics view.
  /// Every model that is connected gets automatically
  /// resizes to fit the graphics view.
  std::vector<QtModel*> connected_models;
  /// The controller instance.
  QtKeyController controller;
public:
  /// Constructs the graphics view.
  /// @param parent A pointer to the parent widget.
  GraphicsView(QWidget* parent) : QGraphicsView(parent), controller(nullptr) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);
  }
  /// Connects a model to the event system.
  /// @param model The model to connect.
  void connect(QtModel* model) {
    connected_models.emplace_back(model);
  }
  /// Accesses a pointer to the controller.
  Controller* get_controller() noexcept {
    return &controller;
  }
protected:
  /// Handles a key press event, updating controller states.
  void keyPressEvent(QKeyEvent* event) override {
    controller.handle_key_press(event);
    return QGraphicsView::keyPressEvent(event);
  }
  /// Handles a key release event, updating controller states.
  void keyReleaseEvent(QKeyEvent* event) override {
    controller.handle_key_release(event);
    return QGraphicsView::keyReleaseEvent(event);
  }
  /// Overrides the resize event handler
  /// so that the engine can also handle
  /// resize events.
  void resizeEvent(QResizeEvent* event) override {

    auto size = event->size();

    for (auto* model : connected_models) {
      model->resize(size);
    }

    QGraphicsView::resizeEvent(event);
  }
};

/// Implements the Qt target interface.
class QtTargetImpl final : public QtTarget {
  /// The window getting rendered to.
  ScopedPtr<GraphicsView> graphics_view;
public:
  /// Constructs a new instance of a Qt target.
  /// @param parent A pointer to the parent widget.
  QtTargetImpl(QWidget* parent)  : graphics_view(new GraphicsView(parent)) { }
  /// Connects a model to the resize event.
  void connect_model(QtModel* model) override {
    graphics_view->connect(model);
  }
  /// Accesses a pointer to the graphics view.
  QGraphicsView* get_graphics_view() override {
    return graphics_view.get();
  }
  /// Gets a pointer to the controller for the window.
  Controller* get_controller() noexcept override {
    return graphics_view->get_controller();
  }
  /// Shows the graphics window.
  void show() override {
    graphics_view->show();
  }
  /// Hides the graphics window.
  void hide() override {
    graphics_view->hide();
  }
};

} // namespace

ScopedPtr<QtTarget> QtTarget::make(QWidget* parent) {
  return new QtTargetImpl(parent);
}

} // namespace herald
