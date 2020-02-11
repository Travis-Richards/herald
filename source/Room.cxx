#include "Room.h"

#include "Matrix.h"
#include "Tile.h"

namespace {

/// Implements the room interface.
class RoomImpl final : public Room {
  /// The tiles part of the room.
  std::vector<Tile> tiles;
  /// The width of the room, in
  /// terms of tiles.
  int w;
  /// The height of the room, in
  /// terms of tiles.
  int h;
public:
  /// Constructs a new room instance.
  /// @param parent A pointer to the parent object.
  RoomImpl(QObject* parent) : Room(parent) {
    w = 0;
    h = 0;
  }
  /// Clears the tile mod flags.
  void clear_mod_flags() noexcept override {
    for (auto& tile : tiles) {
      tile.clear_mod_flag();
    }
  }
  /// Accesses a tile at specified coordinates.
  const Tile* get_tile(int x, int y) const noexcept override {

    if ((x < 0)
     || (y < 0)
     || (x >= w)
     || (y >= h)) {
      return nullptr;
    }

    return &tiles[(y * w) + x];
  }
  /// Accesses the height of the room.
  int height() const noexcept override {
    return h;
  }
  /// Maps the animation matrix onto the tiles.
  /// @param matrix The animation matrix to map onto the tiles.
  void map_animation_matrix(const Matrix& matrix) override {
    for (int y = 0; y < matrix.height(); y++) {
      for (int x = 0; x < matrix.width(); x++) {
        set_animation_of(x, y, matrix.at(x, y));
      }
    }
  }
  /// Resizes the room.
  /// @param width The new width of the room.
  /// @param height The new height of the room.
  void resize(int width, int height) override {
    w = width;
    h = height;
    tiles.resize(w * h);
  }
  void set_mod_flags() noexcept override {
    for (auto& tile : tiles) {
      tile.set_mod_flag();
    }
  }
  /// Updates the active frame of all tiles containing a particular texture.
  void update_tile_frames(int animation, int frame) noexcept override {
    for (auto& tile : tiles) {
      if (tile.has_animation(animation)) {
        tile.change_frame(frame);
      }
    }
  }
  /// Accesses the width of the room.
  int width() const noexcept override {
    return w;
  }
protected:
  /// Sets the animation of a tile.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  /// @param animation The animation index to assign.
  void set_animation_of(int x, int y, int animation) {
    modify(x, y, [animation](Tile& tile) {
      tile.change_animation(animation);
    });
  }
  /// Modifies a tile.
  /// @tparam Mutator The type of the mutator lambda.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  /// @param mutator The tile mutator instance.
  template <typename Mutator>
  void modify(int x, int y, Mutator mutator) {
    if ((x < w) && (y < h)) {
      mutator(tiles[(y * w) + x]);
    }
  }
};

} // namespace

Room* Room::make(QObject* parent) {
  return new RoomImpl(parent);
}
