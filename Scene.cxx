#include "Scene.h"

#include <QBrush>
#include <QList>
#include <QPixmap>

/// Represents an arbitrary texture.
/// Since some images may fail to load
/// from the game, a texture also has a flag
/// to indicate if it is valid or not.
class Texture final : public QObject {
  /// The brush to paint the texture with.
  QBrush brush;
  /// Whether or not this is a valid texture.
  bool valid;
  /// Constructs the texture internally,
  /// with a flag to indicate if it is valid or not.
  Texture(const QBrush& b, bool v, QObject* parent)
    : QObject(parent), brush(b), valid(v) {}
public:
  /// No default constructors allowed.
  Texture() = delete;
  /// Creates a texture from an image.
  /// @param path The path of the image to
  /// generate the texture with.
  /// @param parent A pointer to the parent object.
  /// @returns A new texture instance.
  static Texture* from_image(const QString& path, QObject* parent) {
    QPixmap pixmap;
    if (pixmap.load(path)) {
      return new Texture(QBrush(pixmap), true, parent);
    } else {
      return new Texture(QBrush(), false, parent);
    }
  }
  /// Accesses the brush for the texture.
  const QBrush& get_brush() const noexcept {
    return brush;
  }
  /// Indicates if the texture is valid or not.
  bool is_valid() const noexcept {
    return valid;
  }
};

/// This class contains a list of textures.
/// Each texture in the list may be valid or
/// invalid (in the case of a missing image.)
/// Invalid textures are kept for indexing purposes.
class TextureList : public QObject {
public:
  /// Constructs an empty texture list.
  /// @param parent A pointer to the parent object.
  TextureList(QObject* parent)
    : QObject(parent), default_brush(Qt::black) {}
  /// Accesses the brush of a texture.
  /// @param id The ID of the texture
  /// to get the brush of. This is not
  /// bounds checked and should be before
  /// calling this function.
  const QBrush& brush_of(int id) {

    if (id >= size()) {
      return default_brush;
    }

    const auto* texture = textures[id];

    if (!texture->is_valid()) {
      return default_brush;
    }

    return texture->get_brush();
  }
  /// Loads an image texture.
  /// @param path The path to the image to load.
  void load(const QString& path);
  /// Indicates the number of textures in the list.
  int size() const noexcept {
    return textures.size();
  }
private:
  /// The default brush if a texture ID
  /// is out of bounds or the texture is
  /// invalid.
  QBrush default_brush;
  /// The textures added to the list.
  QList<Texture*> textures;
};

void TextureList::load(const QString&path) {
  textures.push_back(Texture::from_image(path, this));
}

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
  textures = new TextureList(this);
  setBackgroundBrush(QBrush(Qt::black));
}

void Scene::load_image_texture(const QString& path) {
  textures->load(path);
}

void Scene::set_background_texture(int id) {
  setBackgroundBrush(textures->brush_of(id));
}
