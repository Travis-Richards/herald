#pragma once

#include "Room.h"

class QGraphicsItem;
class QSize;

namespace herald {

template <typename T>
class ScopedPtr;

class AnimationTable;
class QtTextureTable;

/// The Qt interface for a room.
class QtRoom : public Room {
public:
  /// Creates a new Qt room instance.
  /// @param parent A pointer to the parent graphics item.
  /// @returns A new Qt room instance.
  static ScopedPtr<QtRoom> make(QGraphicsItem* parent);
  /// Just a stub.
  virtual ~QtRoom() {}
  /// Accesses the graphical item representing the room.
  virtual QGraphicsItem* get_graphics_item() = 0;
  /// Accesses the tile size of the room.
  /// @returns The tile size of the room.
  virtual QSize get_tile_size() const noexcept = 0;
  /// Handles a window resize event.
  /// @param size The window size to scale to.
  virtual void handle_resize(const QSize& size) = 0;
  /// Updates the texture indices used for each tile in the room.
  /// @param ellapsed_ms The updated number of ellapsed milliseconds.
  /// @param animations A reference to the animation table.
  virtual void update_texture_indices(std::size_t ellapsed_ms, const AnimationTable& animations) = 0;
  /// Updates the textures assigned to each of the tiles.
  /// This should be called after the frame indices have been updated.
  /// @param textures The textures to map onto the tiles.
  virtual void update_textures(const QtTextureTable& textures) = 0;
};

} // namespace herald
