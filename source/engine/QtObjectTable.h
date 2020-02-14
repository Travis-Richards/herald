#pragma once

#include "ObjectTable.h"

#include <cstddef>

class QGraphicsItem;
class QSize;

namespace herald {

template <typename T>
class ScopedPtr;

class AnimationTable;
class QtTextureTable;

/// The Qt interface for an object table.
class QtObjectTable : public ObjectTable {
public:
  /// Creates a new Qt implementation of an object table.
  /// @param parent A pointer to the parent graphics item.
  /// @returns A new Qt implementation of an object table.
  static ScopedPtr<QtObjectTable> make(QGraphicsItem* parent);
  /// Just a stub.
  virtual ~QtObjectTable() {}
  /// Accesses a pointer to the graphics item.
  /// @returns A pointer to the graphics item.
  virtual QGraphicsItem* get_graphics_item() = 0;
  /// Resizes the objects in the object table to a new standard size.
  /// @param size The new size standard to adjust the objects to.
  virtual void resize_standard(const QSize& size) = 0;
  /// Updates the positions of the objects on the scene.
  /// @param tile_size The size of a tile, used as a reference for tableping coordinates.
  virtual void update_positions(const QSize& tile_size) = 0;
  /// Updates the texture indices for the objects.
  /// @param ellapsed_ms The total number of ellapsed milliseconds during game play.
  /// @param animations The animation table to get the texture indices from.
  virtual void update_texture_indices(std::size_t ellapsed_ms, const AnimationTable& animations) = 0;
  /// Updates the textures assigned to each of the objects.
  /// @param textures The texture table to get the textures from.
  virtual void update_textures(const QtTextureTable& textures) = 0;
};

} // namespace herald
