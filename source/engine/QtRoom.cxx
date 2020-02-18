#include "QtRoom.h"

#include <herald/ScopedPtr.h>
#include <herald/Vector.h>

#include "QtModel.h"
#include "QtTile.h"

#include <QGraphicsItemGroup>

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
  /// Gets the tile size of the room.
  /// @returns The tile size of the room.
  QSize get_tile_size() const noexcept override {
    if (!width() || !height()) {
      return QSize(0, 0);
    } else {
      return QSize(display_size.width()  / width(),
                   display_size.height() / height());
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
  /// Updates the texture indices for the tiles.
  /// @param ellapsed_ms The updated timeline value.
  /// @param animation A reference to the animation table to get the texture indices from.
  /// @returns A list of indices indicating which tiles were updated.
  Vector<std::size_t> update_texture_indices(std::size_t ellapsed_ms, const AnimationTable& animations) override {

    Vector<std::size_t> update_list;

    for (std::size_t i = 0; i < tiles.size(); i++) {
      if (tiles[i]->update_texture_index(ellapsed_ms, animations)) {
        update_list.push_back(i);
      }
    }

    return update_list;
  }
  /// Updates the textures mapped onto each of the tiles.
  /// @param mod_list A list of which tiles had their texture indices changed.
  /// @param textures The texture table to update the tiles with.
  void update_textures(const Vector<std::size_t>& mod_list, const QtTextureTable& textures) override {
    for (const auto& tile_index : mod_list) {
      tiles[tile_index]->update_texture(textures);
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
