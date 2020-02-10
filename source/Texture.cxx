#include "Texture.h"

#include <QBrush>
#include <QPixmap>

namespace {

/// An implementation of the texture interface.
class TextureImpl final : public Texture {
  /// The pixmap containing the texture data.
  QPixmap pixmap;
public:
  /// Constructs a texture from an existing pixmap.
  TextureImpl(const QPixmap& pixmap_, QObject* parent)
    : Texture(parent), pixmap(pixmap_) {}
  /// Constructs the texture instance.
  TextureImpl(const QString& path, QObject* parent)
    : Texture(parent), pixmap(path) {}
  /// Converts the texture to a brush.
  /// @param tile_size The size of the tile that
  /// teh texture is for.
  QBrush as_brush(const QSize& tile_size) const override {
    return QBrush(pixmap.scaled(tile_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  }
};

} // namespace

Texture* Texture::make(const QPixmap& pixmap, QObject* parent) {
  return new TextureImpl(pixmap, parent);
}

Texture* Texture::open(const QString& path, QObject* parent) {
  return new TextureImpl(path, parent);
}
