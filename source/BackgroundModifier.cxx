#include "BackgroundModifier.h"

#include "Background.h"
#include "Interpreter.h"
#include "Scene.h"

#include "lang/ParseTree.h"
#include "lang/Parser.h"
#include "lang/ScopedPtr.h"

namespace {

/// Implements background modification.
class BackgroundModifier final : public Interpreter {
  /// The scene whose background
  /// is getting modified.
  Scene* scene;
public:
  /// Constructs a background modifier instance.
  /// @param s The scene to modify the background of.
  /// @param parent A pointer to the parent object.
  BackgroundModifier(Scene* s, QObject* parent) : Interpreter(parent), scene(s) { }
  /// Inteprets the response of
  /// the background modification command.
  /// @returns True on success, false on failure.
  bool interpret(Parser& parser) override {

    auto animation = parser.parse_integer();
    if (!check(animation)) {
      return false;
    }

    auto frame_offset = parser.parse_integer();
    if (!check(frame_offset)) {
      return false;
    }

    int animation_value = 0;

    if (!animation.to_signed_value(animation_value)) {
      return false;
    }

    int frame_offset_value = 0;

    if (!frame_offset.to_signed_value(frame_offset_value)) {
      return false;
    }

    auto* background = scene->get_background();
    background->set_animation(animation_value);
    background->set_frame_offset(frame_offset_value);

    return true;
  }
};

} // namespace

Interpreter* make_background_modifier(Scene* scene, QObject* parent) {
  return new BackgroundModifier(scene, parent);
}
