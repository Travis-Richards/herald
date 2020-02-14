#include "QtBackground.h"

#include "ScopedPtr.h"

#include <QGraphicsRectItem>
#include <QPen>

namespace herald {

namespace {

/// An implementation of the background using Qt.
class QtBackgroundImpl final : public QtBackground {
  /// A pointer to the graphical rectangle item.
  ScopedPtr<QGraphicsRectItem> item;
public:
  /// Constructs a new instance of the Qt background animation.
  /// @param parent A pointer to the graphical parent item.
  QtBackgroundImpl(QGraphicsItem* parent) : item(new QGraphicsRectItem(parent)) {
    item->setPen(Qt::NoPen);
    item->setRect(0, 0, 1, 1);
    item->setZValue(-1);
    item->setBrush(QBrush(Qt::black));
  }
  /// Handles a window resize event.
  /// @param size The size to scale to.
  void handle_resize(const QSize& size) override {
    item->setRect(0, 0, size.width(), size.height());
  }
  /// Accesses a pointer to the graphics item.
  QGraphicsItem* get_graphics_item() override {
    return item.get();
  }
};

} // namespace

ScopedPtr<QtBackground> QtBackground::make(QGraphicsItem* parent) {
  return new QtBackgroundImpl(parent);
}

} // namespace herald
