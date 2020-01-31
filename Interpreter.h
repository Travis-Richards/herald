#pragma once

#include <QObject>

class Scene;

/// An interpreter is used to handle
/// the response of the game when
/// issued a command.
class Interpreter : public QObject {
public:
  /// Creates a menu builder interpreter.
  /// @param scene The scene to draw the menu onto.
  /// @param parent A pointer to the parent object.
  /// @returns A pointer to a new interpreter instance.
  static Interpreter* make_menu_builder(Scene* scene, QObject* parent);
  /// Constructs the base interpreter.
  /// @param object A pointer to the parent object.
  Interpreter(QObject* parent);
  /// Just a stub.
  virtual ~Interpreter() {}
  /// Indicates if the interpreter is has
  /// reached the end of the response text.
  virtual bool done() const noexcept {
    return finish_reached;
  }
  /// Interprets the response sent from the game.
  /// @param text The text to interpret from the game.
  virtual void interpret(const QString& text);
protected slots:
  /// Called when the response is completed.
  virtual void finish() {
    finish_reached = true;
  }
  /// Loads an image as a texture.
  /// @param path The path of the image to load.
  virtual void load_image_texture(const QString& path) = 0;
  /// Sets the background the scene.
  /// @param texture_id The ID of the texture to assign.
  virtual void set_background(int texture_id) = 0;
private:
  /// Whether or not the "finish" part
  /// of the response has been reached.
  bool finish_reached;
};
