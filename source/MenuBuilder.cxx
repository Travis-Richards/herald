#include "MenuBuilder.h"

#include "lang/Interpreter.h"
#include "lang/ParseTree.h"
#include "lang/Parser.h"

#include "Scene.h"

namespace {

/// Used for constructing the game menu.
class MenuBuilder final : public Interpreter {
  /// The scene to build the menu onto.
  Scene* scene;
public:
  /// Constructs an instance of the menu builder.
  /// @param s The scene to put the menu onto.
  /// @param parent A pointer to the parent object.
  MenuBuilder(Scene* s, QObject* parent)
    : Interpreter(parent), scene(s) {}
  /// Interpreters the response to the command.
  void interpret(Parser&) override {

  }
};

} // namespace

Interpreter* make_menu_builder(Scene* scene, QObject* parent) {
  return new MenuBuilder(scene, parent);
}
