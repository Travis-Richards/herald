#include "MenuBuilder.h"

#include "lang/Interpreter.h"
#include "lang/ParseTree.h"
#include "lang/Parser.h"

#include "Scene.h"

#include <QDir>
#include <QString>

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
  /// Assigns the root path to open files from.
  void set_root_path(const QString& path) override {
    texture_prefix = QDir::cleanPath(path) + QDir::separator() + "textures" + QDir::separator();
  }
  /// Defines a color texture.
  void interpret(const ColorTextureDecl& color_texture_decl) override {
    (void)color_texture_decl;
  }
  /// Called when an image texture declaration is found.
  /// @param path The path to the image texture to set.
  virtual void interpret(const ImageTextureDecl& image_texture_decl) override {
    scene->load_texture(texture_prefix + image_texture_decl.get_image_path());
  }
  /// Adds a polygon object to the scene.
  void interpret(const DrawBoxStmt& draw_box_stmt) override {
    (void)draw_box_stmt;
  }
protected slots:
  /// Called when a background texture is being set.
  /// @param texture_id The ID of the texture to set the background to.
  virtual void set_background(int texture_id) override {
    (void)texture_id;
  }
  /// Called when a "finish" statement is on.
  virtual void finish() override {
    Interpreter::finish();
  }
private:
  /// The scene to build.
  Scene* scene;
  /// The path to get texture images from.
  QString texture_prefix;
};

} // namespace

Interpreter* make_menu_builder(Scene* scene, QObject* parent) {
  return new SceneBuilder(scene, parent);
}
