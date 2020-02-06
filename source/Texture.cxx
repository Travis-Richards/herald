#include "Texture.h"

#include <QBrush>
#include <QPixmap>

namespace {

/// An implementation of the texture interface.
class TextureImpl final : public Texture {
  /// The pixmap containing the texture data.
  QPixmap pixmap;
public:
  /// Constructs the texture instance.
  TextureImpl(const QString& path, QObject* parent)
    : Texture(parent), pixmap(path) {}
  /// Converts the texture to a brush.
  /// @param tile_size The size of the tile that
  /// teh texture is for.
  QBrush as_brush(const QSize& tile_size) override {
    return QBrush(pixmap.scaled(tile_size));
  }
};

} // namespace

Texture* Texture::open(const QString& path, QObject* parent) {
  return new TextureImpl(path, parent);
}