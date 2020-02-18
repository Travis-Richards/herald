#include "ObjectMapBuilder.h"

#include <herald/Model.h>
#include <herald/Object.h>
#include <herald/ObjectTable.h>
#include <herald/ScopedPtr.h>
#include <herald/Vec2f.h>

#include "Interpreter.h"

#include <herald/protocol/ParseTree.h>
#include <herald/protocol/Parser.h>

using namespace herald;

namespace {

/// Used to insert objects into the model.
class ObjectTableBuilder final : public Interpreter {
  /// The model to add the objects to.
  Model* model;
public:
  /// Constructs an object table builder.
  /// @param m The model to put the objects into.
  /// @param parent A pointer to the parent object.
  ObjectTableBuilder(Model* m, QObject* parent) : Interpreter(parent), model(m) { }
  /// Interprets the response to the "add objects" command.
  /// @param parser The parser to parse the response with.
  /// @returns True on success, false on failure.
  bool interpret(protocol::Parser& parser) override {

    auto count_node = parser.parse_integer();
    if (!check(count_node)) {
      return false;
    }

    int signed_count = 0;

    if (!count_node.to_signed_value(signed_count)) {
      return false;
    } else if (signed_count <= 0) {
      return true;
    }

    std::size_t count = (std::size_t) signed_count;

    auto* object_table = model->get_object_table();

    object_table->resize(count);

    for (std::size_t i = 0; i < count; i++) {

      auto* object = object_table->at(i);

      if (!parse_object(parser, object)) {
        break;
      }
    }

    return true;
  }
protected:
  /// Parses a single object.
  /// @param parser The parser to parse the object with.
  bool parse_object(protocol::Parser& parser, Object* object) {

    auto x_node = parser.parse_integer();
    auto y_node = parser.parse_integer();
    auto action_node = parser.parse_integer();

    if (!check(x_node)
     || !check(y_node)
     || !check(action_node)) {
      return false;
    }

    int x = 0;
    int y = 0;
    int action = 0;

    if (!x_node.to_signed_value(x)
     || !y_node.to_signed_value(y)
     || !action_node.to_signed_value(action)) {
      return false;
    }

    object->translate(Vec2f(x, y));
    object->set_action_index((std::size_t) action);

    return true;
  }
};

} // namespace

Interpreter* make_object_table_builder(Model* model, QObject* parent) {
  return new ObjectTableBuilder(model, parent);
}
