#include "GraphicsView.h"

#include <QResizeEvent>

namespace herald {

namespace tk {

void GraphicsView::resizeEvent(QResizeEvent* event) {
  emit resized(event->size());
  QGraphicsView::resizeEvent(event);
}

} // namespace tk

} // namespace herald
