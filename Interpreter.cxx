#include "Interpreter.h"

#include "Parser.h"
#include "Scene.h"

namespace {

/// This is the base for any class
/// that builds a type of scene.
class SceneBuilder : public Interpreter {
public:
  /// Constructs a new scene builder instance.
  /// @param scene_ The scene to receive the data.
  /// @param parent A pointer to a parent object.
  SceneBuilder(Scene* scene_, QObject* parent)
    : Interpreter(parent), scene(scene_) {}
protected slots:
  /// Called when a background texture is being set.
  /// @param texture_id The ID of the texture to set the background to.
  virtual void set_background(int texture_id) {
    scene->set_background_texture(texture_id);
  }
  /// Called when an image texture statement is found.
  /// @param path The path to the image texture to set.
  virtual void load_image_texture(const QString& path) override {
    scene->load_image_texture(path);
  }
  /// Called when a "finish" statement is on.
  virtual void finish() override {
    Interpreter::finish();
  }
private:
  /// The scene to build.
  Scene* scene;
};

} // namespace

Interpreter* Interpreter::make_menu_builder(Scene* scene, QObject* parent) {
  return new SceneBuilder(scene, parent);
}

Interpreter::Interpreter(QObject* parent) : QObject(parent), finish_reached(false) {

}

void Interpreter::interpret(const QString& text) {

  auto parser = new Parser(text, this);

  connect(parser, &Parser::found_background, this, &Interpreter::set_background);
  connect(parser, &Parser::found_finish, this, &Interpreter::finish);
  connect(parser, &Parser::found_image_texture, this, &Interpreter::load_image_texture);

  parser->parse();
}
