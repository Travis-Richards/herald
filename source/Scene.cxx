#include "Scene.h"

#include "lang/Matrix.h"

#include "Texture.h"
#include "TextureList.h"

#include <QRectF>
#include <QSize>

#include <vector>

namespace {

/// Contains data for one tile.
struct Tile final {
  /// The ID of the texture used
  /// to render this tile.
  int texture_id;
  /// Constructs a tile instance.
  constexpr Tile() noexcept
    : texture_id(0) {}
};

/// An implementation of the scene interface.
class SceneImpl final : public Scene {
  /// The textures to be used by the scene.
  TextureList* textures;
  /// The size of the window being rendered to.
  QSize window_size;
  /// The size of the room being rendered.
  QSize room_size;
  /// The tiles part of the scene.
  std::vector<Tile> tiles;
public:
  /// Constructs an instance of the scene implementation.
  /// @param parent A pointer to the parent object.
  SceneImpl(QObject* parent) : Scene(parent), textures(nullptr) {
    textures = TextureList::make(this);
  }
  /// Builds the room with a texture matrix.
  /// @param texture_matrix The texture matrix to assign the room.
  void build_room(const Matrix& texture_matrix) override {

    auto w = texture_matrix.width();
    auto h = texture_matrix.height();

    room_size = QSize(w, h);

    tiles.resize(w * h);

    int tile_index = 0;

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        tiles[tile_index++].texture_id = texture_matrix.at(x, y);
      }
    }

    add_tile_items();
  }
  /// Loads a texture to be used by the tiles.
  /// @param path The path of the texture to load.
  void load_texture(const QString& path) override {
    textures->add(Texture::open(path, this));
  }
protected:
  /// Calculates the size of a tile.
  /// @returns The size, in pixels, of a tile.
  QSize calc_tile_size() const noexcept {
    return QSize(window_size.width() / room_size.width(),
                 window_size.height() / room_size.height());
  }
  /// Creates a rectangle that represents a certain tile.
  /// @param tile_size The size of the tile.
  /// @param x The X coordinate to place the tile at.
  /// @param y The Y coordinate to place the tile at.
  /// @returns A rectangle containing the tile area.
  QRectF make_tile_rect(const QSize& tile_size, int x, int y) {
    return QRectF(x * tile_size.width(),
                  y * tile_size.height(),
                  tile_size.width(),
                  tile_size.height());
  }
  /// Adds the tiles to the scene as graphic items.
  void add_tile_items() {

    auto tile_size = calc_tile_size();

    for (int y = 0; y < room_size.height(); y++) {

      for (int x = 0; x < room_size.width(); x++) {

        auto tile_rect = make_tile_rect(tile_size, x, y);

        addRect(tile_rect, QPen(Qt::NoPen), brush_of_tile(tile_size, x, y));
      }
    }
  }
  /// Gets the brush for a certain tile.
  /// @param tile_size The size of the tile that the brush is for.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  QBrush brush_of_tile(const QSize& tile_size, int x, int y) {
    if ((x < room_size.width()) && (y < room_size.height())) {
      return brush_of_texture(tile_size, tiles[(y * room_size.width()) + x].texture_id);
    } else {
      return QBrush();
    }
  }
  /// Gets the brush for a certain texture.
  /// @param tile_size The size of the tile the texture is getting made for.
  /// @param texture_id The ID of the texture to get the brush of.
  /// @returns The brush for the specified texture.
  QBrush brush_of_texture(const QSize& tile_size, int texture_id) {
    if (texture_id < textures->size()) {
      auto* texture = textures->at(texture_id);
      if (texture) {
        return texture->as_brush(tile_size);
      } else {
        return QBrush();
      }
    } else {
      return QBrush();
    }
  }
  /// Handles resizing of the scene.
  void resize(const QSize& size) override {

    window_size = size;

    clear();

    add_tile_items();
  }
};

} // namespace

Scene* Scene::make(QObject* parent) {
  return new SceneImpl(parent);
}
