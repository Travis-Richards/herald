#include "BackgroundModifier.h"

#include "Background.h"
#include "Scene.h"

#include "lang/Interpreter.h"
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
  void interpret(Parser& parser) override {

    auto response = parser.parse_set_background_response();
    if (response) {
      handle(*response);
    }

    emit finished();
  }
protected:
  /// Handles the command response.
  void handle(const SetBackgroundResponse& response) {
    auto* background = scene->get_background();
    background->set_animation(response.get_animation());
    background->set_frame_offset(response.get_frame());
  }
};

} // namespace

Interpreter* make_background_modifier(Scene* scene, QObject* parent) {
  return new BackgroundModifier(scene, parent);
}
