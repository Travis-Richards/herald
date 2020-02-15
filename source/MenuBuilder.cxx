#include "MenuBuilder.h"

#include "Interpreter.h"

#include "protocol/ParseTree.h"
#include "protocol/Parser.h"

namespace {

using namespace herald;

/// Used for constructing the game menu.
class MenuBuilder final : public Interpreter {
public:
  /// Constructs an instance of the menu builder.
  /// @param m The model to put the menu onto.
  /// @param parent A pointer to the parent object.
  MenuBuilder(Model* s, QObject* parent) : Interpreter(parent) {
    (void)s;
  }
  /// Interpreters the response to the command.
  /// @returns True on success, false on failure.
  bool interpret(Parser&) override {
    return true;
  }
};

} // namespace

Interpreter* make_menu_builder(Model* model, QObject* parent) {
  return new MenuBuilder(model, parent);
}
