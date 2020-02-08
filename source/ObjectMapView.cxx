#include "ObjectMapView.h"

namespace {

/// The implementation of the object map view interface.
class ObjectMapViewImpl final : public ObjectMapView {
public:
  /// Constructs an instance of the object map view implementation.
  ObjectMapViewImpl(QGraphicsItem* parent) : ObjectMapView(parent) {}
  /// Resizes a view of the object map.
  /// @param size The window size to adjust the view to.
  void resize_view(const QSize& size) override {
    (void)size;
  }
};

} // namespace

ObjectMapView* ObjectMapView::make(QGraphicsItem* parent) {
  return new ObjectMapViewImpl(parent);
}
