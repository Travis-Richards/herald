#pragma once

#include <QObject>

class QBrush;
class QString;

/// Used for painting objects or background images.
class Texture : public QObject {
public:
  /// Creates a texture from an image.
  /// @param path The path of the image to open.
  /// @param parent A pointer to the parent object.
  /// @returns A pointer to a new texture instance.
  static Texture* from_image(const QString& path, QObject* parent);
  /// Creates a texture from a simple color.
  /// @param color The color to assign the texture.
  /// @param parent A pointer to a parent object.
  /// @returns A new texture instance.
  static Texture* from_color(const QColor& color, QObject* parent);
  /// Constructs the base texture object.
  /// @param parent A pointer to the parent object.
  Texture(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~Texture() {}
  /// Accesses the brush of the texture.
  virtual const QBrush& get_brush() const noexcept = 0;
};
