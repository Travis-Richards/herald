#include "Scene.h"

#include "Room.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "TextureAnimationGroup.h"
#include "Tile.h"

#include <QRectF>
#include <QSize>

namespace {

/// An implementation of the scene interface.
class SceneImpl final : public Scene {
  /// The textures to be used by the scene.
  TextureAnimationGroup* textures;
  /// The size of the window being rendered to.
  QSize window_size;
  /// A pointer to the room
  /// being rendered.
  Room* room;
public:
  /// Constructs an instance of the scene implementation.
  /// @param parent A pointer to the parent object.
  SceneImpl(QObject* parent) : Scene(parent), textures(nullptr) {

    textures = TextureAnimationGroup::make(this);

    connect(textures, &TextureAnimationGroup::synced, this, &SceneImpl::sync);

    connect(textures, &TextureAnimationGroup::update_frame, this, &SceneImpl::update_animation_frame);

    room = Room::make_null(this);
  }
  /// Assigns a room to the scene.
  /// @param other_room The room to assign.
  void set_room(Room* other_room) override {

    textures->stop();

    delete room;
    room = other_room;
    room->setParent(this);

    add_tile_items();

    textures->start();
  }
  /// Loads a texture to be used by the tiles.
  /// @param path The path of the texture to load.
  void load_texture(const QString& path) override {
    textures->open(path);
  }
protected slots:
  /// Synchronized tile changes with the graphics scene.
  void sync() {
    add_tile_items();
  }
  /// Updates the frame for a particular texture ID.
  /// @param texture The ID of the texture being updated.
  /// @param frame The frame to assign the tile.
  void update_animation_frame(int texture, int frame) {
    room->update_tile_frames(texture, frame);
  }
protected:
  /// Calculates the size of a tile.
  /// @returns The size, in pixels, of a tile.
  QSize calc_tile_size() const noexcept {

    if (room->empty()) {
      return QSize(0, 0);
    }

    return QSize(window_size.width()  / room->width(),
                 window_size.height() / room->height());
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

    clear();

    auto tile_size = calc_tile_size();

    for (int y = 0; y < room->height(); y++) {

      for (int x = 0; x < room->width(); x++) {

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

    const auto* tile = room->get_tile(x, y);
    if (!tile) {
      return QBrush();
    }

    return brush_of_texture(tile_size, tile->get_frame(), tile->get_texture());
  }
  /// Gets the brush for a certain texture.
  /// @param tile_size The size of the tile the texture is getting made for.
  /// @param frame_id The ID of the animation frame to get the brush of.
  /// @param texture_id The ID of the texture to get the brush of.
  /// @returns The brush for the specified texture.
  QBrush brush_of_texture(const QSize& tile_size, int frame_id, int texture_id) {

    if (texture_id >= textures->size()) {
      return QBrush();
    }

    auto* texture_animation = textures->at(texture_id);
    if (!texture_animation) {
      return QBrush();
    }

    auto* texture = texture_animation->get_frame(frame_id);
    if (!texture) {
      return QBrush();
    }

    return texture->as_brush(tile_size);
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
