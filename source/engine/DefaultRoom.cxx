#include "DefaultRoom.h"

#include "DefaultTile.h"
#include "Index.h"
#include "ScopedPtr.h"
#include "Tile.h"

#include <vector>

namespace herald {

namespace {

/// The default implementation of a room.
class DefaultRoomImpl final : public DefaultRoom {
  /// The tiles of the room.
  std::vector<ScopedPtr<DefaultTile>> tiles;
public:
  /// Accesses a non-const tile.
  Tile* at(std::size_t x, std::size_t y) override {
    if ((x < width()) && (y < height())) {
      return tiles.at((y * width()) + x).get();
    } else {
      return Tile::get_null_tile();
    }
  }
  /// Resizes the room.
  /// @param w The width to assign the room.
  /// @param h The height to assign the room.
  void resize(std::size_t w, std::size_t h) override {
    tiles.resize(w * h);
    Room::resize(w, h);
  }
};

} // namespace

ScopedPtr<DefaultRoom> DefaultRoom::make() {
  return new DefaultRoomImpl();
}

} // namespace herald
