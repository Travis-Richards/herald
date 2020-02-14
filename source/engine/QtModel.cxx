#include "QtModel.h"

#include "AnimationTable.h"
#include "Index.h"
#include "ScopedPtr.h"
#include "QtBackground.h"
#include "QtRoom.h"
#include "QtTextureTable.h"

#include <QGraphicsScene>

#include <utility>

namespace herald {

namespace {

/// Implements the Qt model interface.
class QtModelImpl final : public QtModel {
  /// The animation table for the model.
  ScopedPtr<AnimationTable> animations;
  /// The textures for the model.
  ScopedPtr<QtTextureTable> textures;
  /// The graphics scene that everything
  /// is getting put into.
  ScopedPtr<QGraphicsScene> scene;
  /// The Qt implementation of the background.
  ScopedPtr<QtBackground> background;
  /// The room for the model.
  ScopedPtr<QtRoom> room;
  /// The total number of ellapsed milliseconds.
  std::size_t ellapsed_ms;
public:
  /// Constructs a new Qt model instance.
  QtModelImpl()
    : animations(AnimationTable::make()),
      textures(QtTextureTable::make()),
      scene(new QGraphicsScene()),
      background(QtBackground::make(nullptr)),
      room(QtRoom::make(nullptr)),
      ellapsed_ms(0) {

    scene->addItem(background->get_graphics_item());
    scene->addItem(room->get_graphics_item());
  }
  /// Moves the scene forward in time.
  /// @param delta_ms The value to increase the timeline by.
  void advance(std::size_t delta_ms) override {

    ellapsed_ms += delta_ms;

    room->update_frame_indices(ellapsed_ms, *animations);
    room->update_textures(*textures);
  }
  /// Accesses a pointer to the animation table.
  AnimationTable* get_animation_table() override {
    return animations.get();
  }
  /// Accesses a pointer to the background.
  Background* get_background() override {
    return background.get();
  }
  /// Accesses the room.
  /// @returns A pointer to the model's room.
  Room* get_room() override {
    return room.get();
  }
  /// Accesses a pointer to the graphics scene.
  QGraphicsScene* get_scene() override {
    return scene.get();
  }
  /// Accesses a pointer to the texture table.
  TextureTable* get_texture_table() override {
    return textures.get();
  }
  /// Resizes the model.
  /// This is called in the window's event system
  /// so that the model can automatically be scaled.
  /// @param size The size to scale to.
  void resize(const QSize& size) override {
    background->handle_resize(size);
    room->handle_resize(size);
  }
};

} // namespace

ScopedPtr<QtModel> QtModel::make() {
  return new QtModelImpl();
}

} // namespace herald
