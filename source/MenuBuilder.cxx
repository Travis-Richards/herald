#include "MenuBuilder.h"

#include "Interpreter.h"
#include "Scene.h"

#include "lang/ParseTree.h"
#include "lang/Parser.h"

namespace {

/// Used for constructing the game menu.
class MenuBuilder final : public Interpreter {
public:
  /// Constructs an instance of the menu builder.
  /// @param s The scene to put the menu onto.
  /// @param parent A pointer to the parent object.
  MenuBuilder(Scene* s, QObject* parent) : Interpreter(parent) {
    (void)s;
  }
  /// Interpreters the response to the command.
  /// @returns True on success, false on failure.
  bool interpret(Parser&) override {
    return true;
  }
};

} // namespace

Interpreter* make_menu_builder(Scene* scene, QObject* parent) {
  return new MenuBuilder(scene, parent);
}
