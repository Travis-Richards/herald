#include "RoomBuilder.h"

#include "lang/Interpreter.h"
#include "lang/Matrix.h"
#include "lang/ParseTree.h"
#include "lang/Parser.h"

#include "Room.h"
#include "Scene.h"
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
  /// @param width The width of the room.
  /// @param height The height of the room.
  /// @param parent A pointer to the parent object.
  RoomImpl(int width, int height, QObject* parent) : Room(parent), tiles(width * height) {
    w = width;
    h = height;
  }
  /// Maps the texture matrix onto the tiles.
  /// @param matrix The texture matrix to map onto the tiles.
  void map_texture_matrix(const Matrix& matrix) {
    for (int y = 0; y < matrix.height(); y++) {
      for (int x = 0; x < matrix.width(); x++) {
        set_texture_of(x, y, matrix.at(x, y));
      }
    }
  }
  /// Maps the frame matrix onto the tiles.
  /// @param matrix The frame matrix to map onto the tiles.
  void map_frame_matrix(const Matrix& matrix) {
    for (int y = 0; y < matrix.height(); y++) {
      for (int x = 0; x < matrix.width(); x++) {
        set_frame_of(x, y, matrix.at(x, y));
      }
    }
  }
  /// Accesses a tile at specified coordinates.
  const Tile* get_tile(int x, int y) const noexcept override {
    if ((x < w) && (y < h)) {
      return &tiles[(y * w) + x];
    } else {
      return nullptr;
    }
  }
  /// Accesses the width of the room.
  int width() const noexcept override {
    return w;
  }
  /// Accesses the height of the room.
  int height() const noexcept override {
    return h;
  }
  /// Updates the active frame of all tiles containing a particular texture.
  void update_tile_frames(int texture, int frame) noexcept override {
    for (auto& tile : tiles) {
      if (tile.has_texture(texture)) {
        tile.set_frame(frame);
      }
    }
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

/// Used for building rooms.
class RoomBuilder final : public Interpreter {
  /// A pointer to the scene to put the data into.
  Scene* scene;
public:
  /// Constructs an instance to the room builder.
  /// @param scene The scene to put the data into.
  /// @param parent A pointer to the parent object.
  RoomBuilder(Scene* s, QObject* parent) : Interpreter(parent), scene(s) {}
  /// Interprets the response from the game.
  void interpret(Parser& parser) override {

    auto* response = parser.parse_build_room_response();
    if (!response) {
      return;
    }

    handle(*response);

    delete response;
  }
protected:
  /// Handles the build room response.
  /// @param response The response to handle.
  void handle(const BuildRoomResponse& response) {

    const auto& tm = response.get_texture_matrix();

    auto room = new RoomImpl(tm.width(), tm.height(), this);

    room->map_texture_matrix(tm);

    room->map_frame_matrix(response.get_frame_matrix());

    scene->set_room(room);
  }
};

} // namespace

Interpreter* make_room_builder(Scene* scene, QObject* parent) {
  return new RoomBuilder(scene, parent);
}
