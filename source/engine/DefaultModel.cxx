#include "DefaultModel.h"

#include "AnimationTable.h"
#include "DefaultBackground.h"
#include "DefaultRoom.h"
#include "Index.h"
#include "ScopedPtr.h"
#include "TextureTable.h"

#include <utility>

namespace herald {

namespace {

/// Implements the default model interface.
class DefaultModelImpl final : public DefaultModel {
  /// The texture table for the default model.
  ScopedPtr<TextureTable> textures;
  /// The animation table for the default model.
  ScopedPtr<AnimationTable> animations;
  /// The background data.
  ScopedPtr<DefaultBackground> background;
  /// The room containing the tiles.
  ScopedPtr<DefaultRoom> room;
public:
  /// Constructs a new instance of the default model implementation.
  DefaultModelImpl() : textures(nullptr),
                       animations(AnimationTable::make()),
                       background(DefaultBackground::make()),
                       room(DefaultRoom::make()) {}
  /// Accesses a pointer to the animation table.
  AnimationTable* get_animation_table() override {
    return animations.get();
  }
  /// Accesses a pointer to the background.
  /// @returns A pointer to the background.
  Background* get_background() override {
    return background.get();
  }
  /// Accesses a pointer to the room.
  /// @returns A pointer to the room.
  Room* get_room() override {
    return room.get();
  }
  /// Accesses the texture table.
  TextureTable* get_texture_table() override {
    return textures.get();
  }
};

} // namespace

ScopedPtr<DefaultModel> DefaultModel::make() {
  return new DefaultModelImpl();
}

} // namespace herald
