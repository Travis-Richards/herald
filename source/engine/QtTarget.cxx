#include "QtTarget.h"

#include "ScopedPtr.h"
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
public:
  /// Constructs the graphics view.
  /// @param parent A pointer to the parent widget.
  GraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);
  }
  /// Connects a model to the event system.
  /// @param model The model to connect.
  void connect(QtModel* model) {
    connected_models.emplace_back(model);
  }
protected:
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
  /// Accesses a pointer to the graphics view.
  QGraphicsView* get_graphics_view() override {
    return graphics_view.get();
  }
  /// Connects a model to the resize event.
  void connect_model(QtModel* model) override {
    graphics_view->connect(model);
  }
};

} // namespace

ScopedPtr<QtTarget> QtTarget::make(QWidget* parent) {
  return new QtTargetImpl(parent);
}

} // namespace herald
