#include "RoomBuilder.h"

#include "Interpreter.h"
#include "Matrix.h"
#include "ScopedPtr.h"

#include "engine/Model.h"
#include "engine/Room.h"
#include "engine/Tile.h"

#include "protocol/ParseTree.h"
#include "protocol/Parser.h"

using namespace herald;

namespace {

/// Used for building rooms.
class RoomBuilder final : public Interpreter {
  /// A pointer to the model to put the data into.
  Model* model;
public:
  /// Constructs an instance to the room builder.
  /// @param model The model to put the data into.
  /// @param parent A pointer to the parent object.
  RoomBuilder(Model* m, QObject* parent) : Interpreter(parent), model(m) {}
  /// Interprets the response from the game
  /// after issuing a "build room" command.
  /// @returns True on success, false on failure.
  bool interpret(Parser& parser) override {

    auto matrix_node = parser.parse_matrix();
    if (!check(*matrix_node)) {
      return false;
    }

    auto matrix = Matrix::make(*matrix_node);

    auto* room = model->get_room();

    room->resize((std::size_t) matrix->width(),
                 (std::size_t) matrix->height());

    for (std::size_t y = 0; y < matrix->height(); y++) {
      for (std::size_t x = 0; x < matrix->width(); x++) {
        room->at(x, y)->set_animation_index((std::size_t) matrix->at(x, y));
      }
    }

    return true;
  }
};

} // namespace

Interpreter* make_room_builder(Model* model, QObject* parent) {
  return new RoomBuilder(model, parent);
}
