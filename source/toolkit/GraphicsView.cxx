#include "GraphicsView.h"

#include <QResizeEvent>

namespace herald {

namespace tk {

GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
  setFrameStyle(QFrame::NoFrame);
}

void GraphicsView::resizeEvent(QResizeEvent* event) {

  emit resized(event->size());

  QGraphicsView::resizeEvent(event);
}

} // namespace tk

} // namespace herald
