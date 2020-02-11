#include "RoomBuilder.h"

#include "Interpreter.h"
#include "Matrix.h"
#include "Room.h"
#include "Scene.h"
#include "Tile.h"

#include "lang/ParseTree.h"
#include "lang/Parser.h"
#include "lang/ScopedPtr.h"

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
  /// Interprets the response from the game
  /// after issuing a "build room" command.
  /// @returns True on success, false on failure.
  bool interpret(Parser& parser) override {

    auto tm_tree = parser.parse_matrix();
    if (!check(*tm_tree)) {
      return false;
    }

    auto tm = Matrix::make(*tm_tree);

    auto* room = scene->get_room();
    room->resize(tm->width(), tm->height());
    room->map_animation_matrix(*tm);

    return true;
  }
};

} // namespace

Interpreter* make_room_builder(Scene* scene, QObject* parent) {
  return new RoomBuilder(scene, parent);
}
