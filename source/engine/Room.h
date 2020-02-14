#pragma once

#include <cstddef>

namespace herald {

class Tile;

/// A room is a grid of tiles used to create
/// the appearance of a room. Tiles do not move
/// but they may be animated.
class Room {
  /// The width of the room, in terms of tiles.
  std::size_t w;
  /// The heigh of the room, in terms of tiles.
  std::size_t h;
public:
  /// Constructs the base of the room.
  constexpr Room() noexcept : w(0), h(0) {}
  /// Just a stub.
  virtual ~Room() {}
  /// Accesses a tile from the room.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  /// @returns A pointer to the specified tile.
  /// If the tile coordinates are out of bounds,
  /// then a pointer to a null tile instance is
  /// returned instead.
  virtual Tile* at(std::size_t x, std::size_t y) = 0;
  /// Resizes the room.
  /// @param width The width to assign the room.
  /// @param height The height to assign the room.
  virtual void resize(std::size_t width, std::size_t height) {
    w = width;
    h = height;
  }
  /// Accesses the width of the room, in terms of tiles.
  inline constexpr std::size_t width() const noexcept {
    return w;
  }
  /// Accesses the height of the room, in terms of tiles.
  inline constexpr std::size_t height() const noexcept {
    return h;
  }
};

} // namespace herald
