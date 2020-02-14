#include "QtRoom.h"

#include "QtModel.h"
#include "QtTile.h"
#include "ScopedPtr.h"

#include <QGraphicsItemGroup>

#include <vector>

namespace herald {

namespace {

/// An implementation of a Qt room.
class QtRoomImpl final : public QtRoom {
  /// The graphics item group to put the tiles into.
  ScopedPtr<QGraphicsItemGroup> item_group;
  /// The tiles that are part of the room.
  std::vector<ScopedPtr<QtTile>> tiles;
  /// The size of the display, in terms of pixels.
  QSize display_size;
public:
  /// Constructs the room instance.
  /// @param parent A pointer to the parent graphics item.
  QtRoomImpl(QGraphicsItem* parent) : item_group(new QGraphicsItemGroup(parent)), display_size(1, 1) {

  }
  /// Accesses a tile at a specific coordinate.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  /// @returns A pointer to the specified tile.
  Tile* at(std::size_t x, std::size_t y) override {
    if ((x < width()) && (y < height())) {
      return tiles[(y * width()) + x].get();
    } else {
      return Tile::get_null_tile();
    }
  }
  /// Handles a window resize event.
  /// @param size The window size to scale to.
  void handle_resize(const QSize& size) override {

    display_size = size;

    adjust_tile_size();
  }
  /// Accesses a pointer to the graphics item.
  QGraphicsItem* get_graphics_item() override {
    return item_group.get();
  }
  /// Resizes the number of tiles in the room.
  /// @param width The width to assign the room.
  /// @param height The height to assign the room.
  void resize(std::size_t width, std::size_t height) override {

    auto prev_size = tiles.size();
    auto next_size = width * height;

    for (auto i = prev_size; i < next_size; i++) {
      tiles.emplace_back(QtTile::make(item_group.get()));
    }

    Room::resize(width, height);

    adjust_tile_size();
  }
  /// Updates the frame indices for the tiles.
  /// @param ellapsed_ms The updated timeline value.
  /// @param animation A reference to the animation table to get the frame indices from.
  void update_frame_indices(std::size_t ellapsed_ms, const AnimationTable& animations) override {
    for (auto& tile : tiles) {
      tile->update_texture_index(ellapsed_ms, animations);
    }
  }
  /// Updates the textures mapped onto each of the tiles.
  /// @param textures The texture table to update the tiles with.
  void update_textures(const QtTextureTable& textures) override {
    for (auto& tile_ptr : tiles) {
      tile_ptr->update_texture(textures);
    }
  }
protected:
  /// Adjusts the tile size to account for either
  /// a new window size or new room dimensions.
  void adjust_tile_size() {

    auto resize_functor = [this](std::size_t x, std::size_t y, QtTile& tile) {

      QSize tile_size(display_size.width()  / width(),
                      display_size.height() / height());

      QRectF tile_rect(x * tile_size.width(),
                       y * tile_size.height(),
                       tile_size.width(),
                       tile_size.height());

      tile.set_rect(tile_rect);
    };

    iterate(resize_functor);
  }
  /// Iterates the tiles.
  /// @param functor The function to call on each tile.
  /// @returns True on success, false if there was an internal error.
  template <typename Functor>
  bool iterate(Functor functor) {

    if (tiles.size() != (width() * height())) {
      return false;
    }

    for (std::size_t y = 0; y < height(); y++) {
      for (std::size_t x = 0; x < width(); x++) {
        auto& tile_ptr = tiles[(y * width()) + x];
        if (!tile_ptr) {
          continue;
        } else {
          functor(x, y, *tile_ptr);
        }
      }
    }

    return true;
  }
};

} // namespace

ScopedPtr<QtRoom> QtRoom::make(QGraphicsItem* parent) {
  return new QtRoomImpl(parent);
}

} // namespace herald
