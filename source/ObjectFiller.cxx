#include "ObjectFiller.h"

#include "Object.h"
#include "ObjectMap.h"
#include "Scene.h"

#include "lang/Interpreter.h"
#include "lang/ParseTree.h"
#include "lang/Parser.h"
#include "lang/Matrix.h"

namespace {

/// Used to insert objects into a scene.
class ObjectFiller final : public Interpreter {
  /// A pointer to the scene to put the objects into.
  Scene* scene;
public:
  /// Constructs an object filler.
  /// @param scene_ The scene to put the objects into.
  /// @param parent A pointer to the parent object.
  ObjectFiller(Scene* scene_, QObject* parent) : Interpreter(parent), scene(scene_) {

  }
  /// Interprets the response to the "add objects" command.
  /// @param parser The parser to parse the response with.
  void interpret(Parser& parser) override {

    auto* response = parser.parse_fill_objects_response();
    if (response) {
      handle_response(*response);
    }

    emit finished();
  }
protected:
  /// Handles the response sent from the game.
  /// @param response The response sent from the game.
  void handle_response(const FillObjectsResponse& response) {

    auto* object_map = scene->get_object_map();

    const auto& action_matrix = response.get_action_matrix();

    const auto& flag_matrix = response.get_flag_matrix();

    for (int y = 0; y < flag_matrix.width(); y++) {
      for (int x = 0; x < flag_matrix.height(); x++) {

        auto a = action_matrix.at(x, y);
        auto f = flag_matrix.at(x, y);

        if (f) {
          object_map->add(new Object(a, x, y, object_map));
        }
      }
    }
  }
};

} // namespace

Interpreter* make_object_filler(Scene* scene, QObject* parent) {
  return new ObjectFiller(scene, parent);
}
