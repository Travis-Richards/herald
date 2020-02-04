#pragma once

class Interpreter;
class QColor;
class QPointF;
class QString;

/// A node is the base class for
/// any type of statement defined
/// by the protocol.
class Node {
public:
  virtual ~Node() {}
  /// Accepts an interpreter instance.
  /// This allows the interpreter access
  /// to the derived node class.
  virtual void accept(Interpreter& interpreter) const = 0;
};

/// Used to define the texture to be
/// put onto the background.
class BackgroundStmt final : public Node {
  /// The ID of the texture to assign the background.
  int texture_id;
public:
  /// Constructs an instance of a background statement.
  /// @param texture_id_ The ID of the texture being
  /// assigned to the background.
  BackgroundStmt(int texture_id_) noexcept
    : texture_id(texture_id_) {}
  /// Accepts an interpreter.
  void accept(Interpreter& interpreter) const override;
  /// Accesses the ID of the texture
  /// to assign to the background.
  inline int get_texture_id() const noexcept {
    return texture_id;
  }
};

/// A simple color texture declaration.
/// Defines a texture with a single color.
class ColorTextureDecl : public Node {
public:
  /// Just a stub.
  virtual ~ColorTextureDecl() {}
  /// Accepts an interpreter.
  void accept(Interpreter& interpreter) const override;
  /// Accesses the color of the texture.
  virtual const QColor& get_color() const noexcept = 0;
};

/// A box draw request.
/// The box being drawn is an
/// axis-aligned box.
class DrawBoxStmt final : public Node {
public:
  /// Constructs the box draw statement.
  /// @param a The first point of the box.
  /// @param b The second point of the box.
  /// @param texture_id_ The ID to draw onto the box.
  DrawBoxStmt(const QPointF& a_,
              const QPointF& b_,
              int texture_id_);
  /// Releases memory allocated by the statement.
  ~DrawBoxStmt();
  /// Accepts an interpreter.
  void accept(Interpreter& interpreter) const override;
  /// Accesses the first point of the box.
  inline const QPointF& get_point_a() const noexcept {
    return *point_a;
  }
  /// Accesses the second point of the box.
  inline const QPointF& get_point_b() const noexcept {
    return *point_b;
  }
  /// Accesses the ID of the texture
  /// to map onto the rectangle.
  inline int get_texture_id() const noexcept {
    return texture_id;
  }
private:
  /// The first point of the box.
  QPointF* point_a;
  /// The second point of the box.
  QPointF* point_b;
  /// The texture to assign the rectangle.
  int texture_id;
};

/// Indicates that a response is done
/// being constructed.
class FinishStmt final : public Node {
public:
  /// Accepts an interpreter.
  void accept(Interpreter& interpreter) const override;
};

/// Defines a image texture to load for the graphics scene.
class ImageTextureDecl : public Node {
public:
  /// Just a stub.
  virtual ~ImageTextureDecl() {}
  /// Accepts an interpreter.
  void accept(Interpreter& interpreter) const override;
  /// Accesses the path to the image being loaded.
  /// @returns The path of the image to load.
  virtual const QString& get_image_path() const noexcept = 0;
};

