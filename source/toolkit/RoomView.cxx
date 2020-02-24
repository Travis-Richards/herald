#include "RoomView.h"

#include <herald/ScopedPtr.h>

#include "GraphicsView.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>

namespace herald {

namespace tk {

namespace {

/// A tile is a single square that's
/// part of the room view.
class Tile final {
  /// The graphics item for the tile.
  ScopedPtr<QGraphicsRectItem> item;
public:
  /// Constructs a new tile instance.
  /// @param parent A pointer to the parent graphics item.
  Tile(QGraphicsItem* parent = nullptr) : item(new QGraphicsRectItem(parent)) {
    item->setRect(0, 0, 1, 1);
    item->setPen(QPen(Qt::blue));
  }
  /// Accesses a pointer to the graphics item.
  QGraphicsItem* get_item() noexcept {
    return item.get();
  }
  /// Repositions the tile in case of either a window
  /// resize event or a room resize event.
  /// @param x The X coordinate to place the tile at,
  /// in terms of tile positions.
  /// @param y The Y coordinate to place the tile at,
  /// in terms of tile positions.
  /// @param w The width, in pixels, to make the tile.
  /// @param h The height, in pixels, to make the tile.
  void reposition(std::size_t x, std::size_t y,
                  std::size_t w, std::size_t h) {
    item->setRect(x * w, y * h, w, h);
  }
};

/// The implementation of the room view interface.
class RoomViewImpl final : public RoomView {
  /// The scene to be rendered.
  ScopedPtr<QGraphicsScene> scene;
  /// A view of the graph graphics scene.
  ScopedPtr<GraphicsView> view;
  /// Rectangles used to highlight the tiles.
  std::vector<Tile> tiles;
  /// The width of the room, in terms of tiles.
  std::size_t w;
  /// The height of the room, in terms of tiles.
  std::size_t h;
public:
  /// Constructs a new instance of the room view.
  /// @param parent A pointer to the parent widget.
  RoomViewImpl(QWidget* parent) : w(0), h(0) {

    scene = ScopedPtr<QGraphicsScene>(new QGraphicsScene());

    view = ScopedPtr<GraphicsView>(new GraphicsView(scene.get(), parent));

    auto resize_functor = [this](const QSize& size) {
      handle_widget_resize(size);
    };

    QObject::connect(view.get(), &GraphicsView::resized, resize_functor);
  }
  /// Accesses a pointer to the widget.
  QWidget* get_widget() noexcept override {
    return view.get();
  }
  /// Resizes the room.
  /// @param width_ The width to resize to.
  /// @param height_ The height to resize to.
  void resize(std::size_t width_, std::size_t height_) override {

    auto prev_size = w * h;
    auto next_size = width_ * height_;

    w = width_;
    h = height_;

    tiles.resize(w * h);

    for (auto i = prev_size; i < next_size; i++) {
      scene->addItem(tiles[i].get_item());
    }

    handle_widget_resize(view->size());
  }
protected slots:
  /// Handles the resizing of the graphics view.
  /// @param size The size of the graphics view.
  void handle_widget_resize(const QSize& size) {

    auto widget_w = (std::size_t) size.width();
    auto widget_h = (std::size_t) size.height();

    std::size_t tile_w = w ? (widget_w / w) : 0;
    std::size_t tile_h = h ? (widget_h / h) : 0;

    for (std::size_t y = 0; y < h; y++) {
      for (std::size_t x = 0; x < w; x++) {
        tiles[(y * w) + x].reposition(x, y, tile_w, tile_h);
      }
    }
  }
};

} // namespace

ScopedPtr<RoomView> RoomView::make(QWidget* parent) {
  return new RoomViewImpl(parent);
}

} // namespace tk

} // namespace herald
