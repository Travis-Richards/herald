#include "TextureView.h"

#include <herald/ScopedPtr.h>

#include "GraphicsView.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>

namespace herald {

namespace tk {

namespace {

/// The implementation of the texture widget interface.
class TextureViewImpl final : public TextureView {
  /// The texture is inserted here,
  /// as a rectangle.
  ScopedPtr<QGraphicsScene> scene;
  /// The graphics item used to display the texture.
  ScopedPtr<QGraphicsRectItem> texture_item;
  /// The widget used to view the texture.
  ScopedPtr<GraphicsView> view;
  /// The texture currently being displayed.
  QPixmap texture;
  /// The size of the graphics view.
  QSize window_size;
public:
  /// Constructs a new texture widget implementation.
  /// @param parent A pointer to the parent widget.
  TextureViewImpl(QWidget* parent) {

    scene = ScopedPtr<QGraphicsScene>(new QGraphicsScene);
    scene->setBackgroundBrush(QBrush(QColor(128, 128, 128, 32)));

    texture_item = ScopedPtr<QGraphicsRectItem>(new QGraphicsRectItem);

    scene->addItem(texture_item.get());

    view = ScopedPtr<GraphicsView>(new GraphicsView(scene.get(), parent));
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameStyle(QFrame::NoFrame);

    auto resize_functor = [this](const QSize& size) {
      handle_resize(size);
    };

    QObject::connect(view.get(), &GraphicsView::resized, resize_functor);
  }
  /// Accesses a pointer to the graphics view widget.
  QWidget* get_widget() noexcept override {
    return view.get();
  }
  /// Opens a texture to be displayed.
  /// @param data The image file data to display.
  /// @returns True on success, false on failure.
  bool open_texture(const QByteArray& data) override {
    if (texture.loadFromData(data)) {
      adjust_texture_size();
      return true;
    } else {
      return false;
    }
  }
protected:
  /// Handles the resizing of the window.
  /// @param size The new size of the window.
  void handle_resize(const QSize& size) {

    window_size = size;

    if (!texture.isNull()) {
      adjust_texture_size();
    }
  }
  /// Resizes the texture to fit the window.
  void adjust_texture_size() {

    auto window_w = window_size.width();
    auto window_h = window_size.height();

    auto min_size = (window_h < window_w) ? window_h : window_w;
    auto max_size = (window_h > window_w) ? window_h : window_w;

    auto offset = (max_size - min_size) / 2;

    QRectF rect(0, 0, min_size, min_size);

    if (window_w > window_h) {
      rect.setX(offset);
    } else {
      rect.setY(offset);
    }

    QSize size(min_size, min_size);

    texture_item->setRect(rect);
    texture_item->setBrush(texture.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  }
};

} // namespace

ScopedPtr<TextureView> TextureView::make(QWidget* parent) {
  return new TextureViewImpl(parent);
}

} // namespace tk

} // namespace herald
