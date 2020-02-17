#pragma once

#include <herald/Tile.h>

#include <cstddef>

class QGraphicsItem;
class QRectF;

namespace herald {

template <typename T>
class ScopedPtr;

class AnimationTable;
class QtTextureTable;

/// The Qt version of the tile instance.
class QtTile : public Tile {
public:
  /// Creates a new Qt tile.
  /// @param parent A pointer to the parent graphics item.
  /// @returns A new Qt tile instance.
  static ScopedPtr<QtTile> make(QGraphicsItem* parent);
  /// Just a stub.
  virtual ~QtTile() {}
  /// Gets a pointer to the graphics item for this tile.
  /// @returns A pointer to the graphics item.
  virtual QGraphicsItem* get_graphics_item() = 0;
  /// Sets the rectangle shape of the tile.
  /// @param rect The rectangle shape to assign.
  virtual void set_rect(const QRectF& rect) = 0;
  /// Updates the texture index used for the tile.
  /// @param ellapsed_ms The updated timeline duration.
  /// @param animations A reference to the animation
  /// table to get the texture index from.
  virtual void update_texture_index(std::size_t ellapsed_ms, const AnimationTable& animations) = 0;
  /// Updates the texture displayed for the tile.
  /// @param textures The table to get the texture data from.
  virtual void update_texture(const QtTextureTable& textures) = 0;
};

} // namespace herald
