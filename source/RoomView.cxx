#include "RoomView.h"

#include "Room.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "TextureAnimationGroup.h"
#include "Tile.h"

#include <QBrush>
#include <QGraphicsRectItem>
#include <QList>
#include <QPen>
#include <QSize>

namespace {

/// Gets the minimum of two values.
template <typename T>
T min(T a, T b) noexcept {
  return (a < b) ? a : b;
}

/// Used to represent a tile view as
/// an instance of a graphics item.
class TileView final : public QGraphicsRectItem {
public:
  /// Constructs a tile view instance.
  /// @param parent A pointer to the parent item.
  TileView(QGraphicsItem* parent) : QGraphicsRectItem(parent) {
    setPen(QPen(Qt::NoPen));
  }
};

/// Implements the full room view functionality.
class RoomViewImpl final : public RoomView {
  /// The tile graphics items.
  QList<TileView*> tile_items;
  /// The height of the room, in terms of tiles.
  int w;
  /// The width of the room, in terms of tiles.
  int h;
  /// The size of the window displaying the room.
  QSize window_size;
public:
  /// Constructs an instance of the room view implementation.
  /// @param parent A pointer to the parent graphics item.
  RoomViewImpl(QGraphicsItem* parent) : RoomView(parent) {
    w = 0;
    h = 0;
  }
  /// Frames the texture animations onto the room view.
  /// @param room The room containing the tile data.
  /// @param textures The texture animations to get the image data from.
  void map(const Room* room, const TextureAnimationGroup* textures) override {

    auto min_w = min(w, room->width());
    auto min_h = min(h, room->height());

    for (int y = 0; y < min_h; y++) {
      for (int x = 0; x < min_w; x++) {
        map_tile(tile_items[(y * w) + x], room->get_tile(x, y), textures);
      }
    }
  }
  /// Resizes the room view to a certain number of tiles.
  /// @param width The width to assign, in terms of tiles.
  /// @param height The height to assign, in terms of tiles.
  void resize(int width, int height) override {

    auto next_tile_count = width * height;

    auto prev_tile_count = w * h;

    w = width;
    h = height;

    // Handle expansion
    for (int i = prev_tile_count; i < next_tile_count; i++) {
      tile_items.push_back(new TileView(this));
    }

    // Handle collapse
    for (int i = prev_tile_count; i > next_tile_count; i--) {
      delete tile_items[i - 1];
      tile_items.removeAt(i - 1);
    }

    resize_view(window_size);
  }
  /// Resizes the tile sizes to accomodate a new window size.
  /// @param size The new size of the window.
  void resize_view(const QSize& size) override {

    window_size = size;

    int tile_w = w ? (size.width()  / w) : 0;
    int tile_h = h ? (size.height() / h) : 0;

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        get_tile(x, y)->setRect(x * tile_w, y * tile_h, tile_w, tile_h);
      }
    }
  }
protected:
  /// Gets a tile by certain coordinates.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  /// @returns A pointer to the specified tile
  /// on success, a null pointer on failure.
  TileView* get_tile(int x, int y) {
    if ((x < w) && (y < h)) {
      return tile_items[(y * w) + x];
    } else {
      return nullptr;
    }
  }
  /// Maps a single tile onto the room view.
  /// @parma tile_view The tile view to map the texture onto.
  /// @param tile The tile to map.
  /// This may be null in the case
  /// of an out of bounds condition.
  /// @param textures The textures
  /// to get the image data from.
  bool map_tile(TileView* tile_view, const Tile* tile, const TextureAnimationGroup* textures) {

    if (!tile) {
      return false;
    }

    auto texture_id = tile->get_texture();

    auto* texture = textures->at(texture_id);
    if (!texture) {
      return false;
    }

    auto frame_id = tile->get_frame();

    auto* frame = texture->get_frame(frame_id);
    if (!frame) {
      return false;
    }

    auto tile_rect = tile_view->rect();

    auto tile_size = QSize(tile_rect.width(), tile_rect.height());

    tile_view->setBrush(frame->as_brush(tile_size));

    return true;
  }
};

} // namespace

RoomView* RoomView::make(QGraphicsItem* parent) {
  return new RoomViewImpl(parent);
}
