#include "Scene.h"

#include "Texture.h"
#include "TextureList.h"

#include <QSize>

namespace {

/// An implementation of the scene interface.
class SceneImpl final : public Scene {
  /// The size of the map, in tiles.
  QSize map_size;
  /// The size of the level, in tiles.
  QSize level_size;
public:
  /// Constructs an instance of the scene implementation.
  /// @param parent A pointer to the parent object.
  SceneImpl(QObject* parent) : Scene(parent) { }
  /// Assigns the map size.
  void set_map_size(const QSize& map_size_) {
    map_size = map_size_;
  }
  /// Assigns the level size.
  void set_level_size(const QSize& level_size_) {
    level_size = level_size_;
  }
  /// Loads a texture to be used by the tiles.
  /// @param path The path of the texture to load.
  void load_texture(const QString& path) override {
    (void)path;
  }
};

} // namespace

Scene* Scene::make(QObject* parent) {
  return new SceneImpl(parent);
}
