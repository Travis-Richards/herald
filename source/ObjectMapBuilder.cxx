#include "ObjectMapBuilder.h"

#include "Interpreter.h"
#include "Object.h"
#include "ObjectMap.h"
#include "Scene.h"
#include "ScopedPtr.h"

#include "protocol/ParseTree.h"
#include "protocol/Parser.h"

using namespace herald;

namespace {

/// Used to insert objects into a scene.
class ObjectMapBuilder final : public Interpreter {
  /// The scene to add the objects to.
  Scene* scene;
public:
  /// Constructs an object filler.
  /// @param scene_ The scene to put the objects into.
  /// @param parent A pointer to the parent object.
  ObjectMapBuilder(Scene* s, QObject* parent) : Interpreter(parent), scene(s) { }
  /// Interprets the response to the "add objects" command.
  /// @param parser The parser to parse the response with.
  /// @returns True on success, false on failure.
  bool interpret(Parser& parser) override {

    auto count_node = parser.parse_integer();
    if (!check(count_node)) {
      return false;
    }

    int count = 0;

    if (!count_node.to_signed_value(count)) {
      return false;
    } else if (count <= 0) {
      return true;
    }

    auto* object_map = scene->get_object_map();

    for (int i = 0; i < count; i++) {

      Object object(-1, -1, -1);

      if (!parse_object(parser, object)) {
        break;
      } else {
        object_map->add(object);
      }
    }

    return true;
  }
protected:
  /// Parses a single object.
  /// @param parser The parser to parse the object with.
  bool parse_object(Parser& parser, Object& object) {

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

    object = Object(action, x, y);

    return true;
  }
};

} // namespace

Interpreter* make_object_map_builder(Scene* scene, QObject* parent) {
  return new ObjectMapBuilder(scene, parent);
}
