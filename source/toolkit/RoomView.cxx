#include "RoomView.h"

#include "GraphicsView.h"
#include "ScopedPtr.h"

#include <QGraphicsScene>

namespace herald {

namespace tk {

namespace {

/// The implementation of the room view interface.
class RoomViewImpl final : public RoomView {
  /// The scene to be rendered.
  ScopedPtr<QGraphicsScene> scene;
  /// A view of the graph graphics scene.
  ScopedPtr<GraphicsView> view;
public:
  /// Constructs a new instance of the room view.
  /// @param parent A pointer to the parent widget.
  RoomViewImpl(QWidget* parent) {

    scene = ScopedPtr<QGraphicsScene>(new QGraphicsScene());

    view = ScopedPtr<GraphicsView>(new GraphicsView(scene.get(), parent));
  }
  /// Accesses a pointer to the widget.
  QWidget* get_widget() noexcept override {
    return view.get();
  }
};

} // namespace

ScopedPtr<RoomView> RoomView::make(QWidget* parent) {
  return new RoomViewImpl(parent);
}

} // namespace tk

} // namespace herald
