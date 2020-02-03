#include "Texture.h"

#include <QBrush>
#include <QPixmap>

namespace {

/// Implements the texture interface.
class TextureImpl final : public Texture {
  /// The brush to paint the texture with.
  QBrush brush;
public:
  /// Constructs an instance of the texture.
  TextureImpl(const QBrush& b, QObject* parent)
    : Texture(parent), brush(b) {}
  /// Accesses the brush for the texture.
  const QBrush& get_brush() const noexcept override {
    return brush;
  }
};

} // namespace

Texture* Texture::from_image(const QString& path, QObject* parent) {
  QPixmap pixmap(path);
  if (!pixmap) {
    return new TextureImpl(QBrush(), parent);
  } else {
    return new TextureImpl(QBrush(pixmap), parent);
  }
}

Texture* Texture::from_color(const QColor& color, QObject* parent) {
  return new TextureImpl(QBrush(color), parent);
}
