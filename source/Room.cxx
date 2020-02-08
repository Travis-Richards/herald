#include "Room.h"

#include "Tile.h"

#include "lang/Matrix.h"

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
  /// Accesses a tile at specified coordinates.
  const Tile* get_tile(int x, int y) const noexcept override {
    if ((x < w) && (y < h)) {
      return &tiles[(y * w) + x];
    } else {
      return nullptr;
    }
  }
  /// Accesses the height of the room.
  int height() const noexcept override {
    return h;
  }
  /// Maps the texture matrix onto the tiles.
  /// @param matrix The texture matrix to map onto the tiles.
  void map_texture_matrix(const Matrix& matrix) override {
    for (int y = 0; y < matrix.height(); y++) {
      for (int x = 0; x < matrix.width(); x++) {
        set_texture_of(x, y, matrix.at(x, y));
      }
    }
  }
  /// Maps the frame matrix onto the tiles.
  /// @param matrix The frame matrix to map onto the tiles.
  void map_frame_matrix(const Matrix& matrix) override {
    for (int y = 0; y < matrix.height(); y++) {
      for (int x = 0; x < matrix.width(); x++) {
        set_frame_of(x, y, matrix.at(x, y));
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
  /// Updates the active frame of all tiles containing a particular texture.
  void update_tile_frames(int texture, int frame) noexcept override {
    for (auto& tile : tiles) {
      if (tile.has_texture(texture)) {
        tile.set_frame(frame);
      }
    }
  }
  /// Accesses the width of the room.
  int width() const noexcept override {
    return w;
  }
protected:
  /// Sets the frame offset of a tile.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  /// @param frame The frame offset to assign.
  void set_frame_of(int x, int y, int frame) {
    modify(x, y, [frame](Tile& tile) {
      tile.set_frame_offset(frame);
    });
  }
  /// Sets the texture of a tile.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  /// @param texture The texture to assign.
  void set_texture_of(int x, int y, int texture) {
    modify(x, y, [texture](Tile& tile) {
      tile.set_texture(texture);
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
