#pragma once

#include <QObject>

class Node;
class ColorTextureDecl;
class DrawBoxStmt;
class ImageTextureDecl;
class Scene;

/// An interpreter is used to handle
/// the response of the game when
/// issued a command.
class Interpreter : public QObject {
public:
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
  virtual void interpret_line(const QString& text);
  /// Sets the root path of the interpreter.
  /// @param path The root path to get files from.
  virtual void set_root_path(const QString& path) = 0;
  /// Called when the response is completed.
  virtual void finish() {
    finish_reached = true;
  }
  /// Sets the background the scene.
  /// @param texture_id The ID of the texture to assign.
  virtual void set_background(int texture_id) = 0;
  /// Interprets a color texture declaration.
  virtual void interpret(const ColorTextureDecl& color_texture_decl) = 0;
  /// Interprets a draw box statement.
  virtual void interpret(const DrawBoxStmt& draw_box_stmt) = 0;
  /// Interprets an image texture declaration.
  virtual void interpret(const ImageTextureDecl& image_texture_decl) = 0;
protected slots:
  /// Handles an arbitrary node.
  virtual void handle_node(const Node& node);
private:
  /// Whether or not the "finish" part
  /// of the response has been reached.
  bool finish_reached;
};
