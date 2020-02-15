#include "Tile.h"

#include "Index.h"

namespace herald {

namespace {

/// An empty tile instance for returning
/// a tile in an out of bounds condition.
class NullTile final : public Tile {
public:
};

/// An instance of the null tile class.
NullTile null_tile;

} // namespace

Tile* Tile::get_null_tile() noexcept {
  return &null_tile;
}

} // namespace herald
