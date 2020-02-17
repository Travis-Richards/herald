#include "BackgroundModifier.h"

#include <herald/Background.h>
#include <herald/Model.h>
#include <herald/ScopedPtr.h>

#include "Interpreter.h"

#include "protocol/ParseTree.h"
#include "protocol/Parser.h"

namespace {

using namespace herald;

/// Implements background modification.
class BackgroundModifier final : public Interpreter {
  /// The model whose background
  /// is getting modified.
  Model* model;
public:
  /// Constructs a background modifier instance.
  /// @param m The model to modify the background of.
  /// @param parent A pointer to the parent object.
  BackgroundModifier(Model* m, QObject* parent) : Interpreter(parent), model(m) { }
  /// Inteprets the response of
  /// the background modification command.
  /// @returns True on success, false on failure.
  bool interpret(herald::Parser& parser) override {

    auto animation = parser.parse_integer();
    if (!check(animation)) {
      return false;
    }

    int animation_value = 0;

    if (!animation.to_signed_value(animation_value)) {
      return false;
    }

    model->get_background()->set_animation_index((std::size_t) animation_value);

    return true;
  }
};

} // namespace

Interpreter* make_background_modifier(herald::Model* model, QObject* parent) {
  return new BackgroundModifier(model, parent);
}
