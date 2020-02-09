#include "Scene.h"

#include "Background.h"
#include "BackgroundView.h"
#include "ObjectMap.h"
#include "ObjectMapView.h"
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
  /// The background of the scene.
  Background background;
  /// A view of the background.
  BackgroundView* background_view;
  /// The object map for the current room.
  ObjectMap* object_map;
  /// A view of the object map.
  ObjectMapView* object_map_view;
  /// A pointer to the room being rendered.
  Room* room;
  /// A view of the room being rendered.
  RoomView* room_view;
public:
  /// Constructs an instance of the scene implementation.
  /// @param parent A pointer to the parent object.
  SceneImpl(QObject* parent) : Scene(parent), textures(nullptr) {

    background_view = BackgroundView::make(nullptr);

    object_map = ObjectMap::make(this);

    object_map_view = ObjectMapView::make(nullptr);

    textures = TextureAnimationGroup::make(this);
    connect(textures, &TextureAnimationGroup::synced, this, &SceneImpl::sync);
    connect(textures, &TextureAnimationGroup::update_frame, this, &SceneImpl::update_animation_frame);

    room = Room::make(this);

    room_view = RoomView::make(nullptr);

    addItem(background_view);
    addItem(room_view);
    addItem(object_map_view);
  }
  /// Accesses the scene background.
  Background* get_background() override {
    return &background;
  }
  /// Accesses the scene's object map.
  ObjectMap* get_object_map() override {
    return object_map;
  }
  /// Accesses the room of the scene.
  Room* get_room() override {
    return room;
  }
  /// Loads a texture to be used by the tiles.
  /// @param path The path of the texture to load.
  void load_texture(const QString& path) override {
    textures->open(path);
  }
  /// Starts the animation sequence.
  void start() override {
    textures->start();
  }
  /// Pauses the animation sequence.
  void pause() override {
    textures->stop();
  }
  /// Updates the view entities of the scene.
  void update_view() override {

    background_view->map(background, *textures);

    room_view->resize(room->width(), room->height());
    room_view->map(room, textures);
  }
public slots:
  /// Handles resizing of the scene.
  void resize(const QSize& size) override {

    background_view->resize_view(size);
    background_view->map(background, *textures);

    room_view->resize_view(size);
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
};

} // namespace

Scene* Scene::make(QObject* parent) {
  return new SceneImpl(parent);
}
