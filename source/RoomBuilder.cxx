#include "RoomBuilder.h"

#include "lang/Interpreter.h"
#include "lang/Matrix.h"
#include "lang/ParseTree.h"
#include "lang/Parser.h"

#include "Room.h"
#include "Scene.h"
#include "Tile.h"

namespace {

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
    if (response) {
      handle(*response);
    }

    delete response;
  }
protected:
  /// Handles the build room response.
  /// @param response The response to handle.
  void handle(const BuildRoomResponse& response) {

    auto* room = scene->get_room();

    const auto& tm = response.get_texture_matrix();

    room->resize(tm.width(), tm.height());

    room->map_texture_matrix(tm);

    room->map_frame_matrix(response.get_frame_matrix());

    emit finished();
  }
};

} // namespace

Interpreter* make_room_builder(Scene* scene, QObject* parent) {
  return new RoomBuilder(scene, parent);
}
