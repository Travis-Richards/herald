#include "ObjectFiller.h"

#include "Interpreter.h"
#include "Object.h"
#include "ObjectMap.h"
#include "Scene.h"

#include "lang/ParseTree.h"
#include "lang/Parser.h"
#include "lang/ScopedPtr.h"

namespace {

/// Used to insert objects into a scene.
class ObjectFiller final : public Interpreter {
public:
  /// Constructs an object filler.
  /// @param scene_ The scene to put the objects into.
  /// @param parent A pointer to the parent object.
  ObjectFiller(Scene* s, QObject* parent) : Interpreter(parent) {
    (void)s;
  }
  /// Interprets the response to the "add objects" command.
  /// @param parser The parser to parse the response with.
  /// @returns True on success, false on failure.
  bool interpret(Parser& parser) override {
    (void)parser;
    return true;
  }
};

} // namespace

Interpreter* make_object_filler(Scene* scene, QObject* parent) {
  return new ObjectFiller(scene, parent);
}
