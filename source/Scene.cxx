#include "Scene.h"

#include "Room.h"
#include "RoomView.h"
#include "TextureAnimationGroup.h"

#include <QRectF>
#include <QSize>

namespace {

/// An implementation of the scene interface.
class SceneImpl final : public Scene {
  /// The textures to be used by the scene.
  TextureAnimationGroup* textures;
  /// A pointer to the room being rendered.
  Room* room;
  /// A view of the room being rendered.
  RoomView* room_view;
public:
  /// Constructs an instance of the scene implementation.
  /// @param parent A pointer to the parent object.
  SceneImpl(QObject* parent) : Scene(parent), textures(nullptr) {

    textures = TextureAnimationGroup::make(this);

    connect(textures, &TextureAnimationGroup::synced, this, &SceneImpl::sync);

    connect(textures, &TextureAnimationGroup::update_frame, this, &SceneImpl::update_animation_frame);

    room = Room::make_null(this);

    room_view = RoomView::make(nullptr);

    addItem(room_view);
  }
  /// Assigns a room to the scene.
  /// @param other_room The room to assign.
  void set_room(Room* other_room) override {

    textures->stop();

    delete room;
    room = other_room;
    room->setParent(this);

    room_view->resize(room->width(), room->height());

    room_view->map(room, textures);

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
    room_view->map(room, textures);
  }
  /// Updates the frame for a particular texture ID.
  /// @param texture The ID of the texture being updated.
  /// @param frame The frame to assign the tile.
  void update_animation_frame(int texture, int frame) {
    room->update_tile_frames(texture, frame);
  }
protected:
  /// Handles resizing of the scene.
  void resize(const QSize& size) override {
    room_view->resize_view(size);
  }
};

} // namespace

Scene* Scene::make(QObject* parent) {
  return new SceneImpl(parent);
}
