#pragma once

#include <QGraphicsView>

namespace herald {

namespace tk {

/// Derived graphics view class in order
/// to emit a signal when the graphics view is resized.
class GraphicsView final : public QGraphicsView {
  Q_OBJECT
public:
  /// Constructs a new instance of a graphics view.
  /// @param scene The scene to be rendered.
  /// @param parent A pointer to the parent widget.
  GraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent) {}
signals:
  /// Emitted when the view is resized.
  /// @param size The size the view is being changed to.
  void resized(const QSize& size);
protected:
  /// Overrides the resize event handler
  /// in order to emit a signal when the view size changes.
  void resizeEvent(QResizeEvent* event) override;
};

} // namespace tk

} // namespace herald
