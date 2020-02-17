#pragma once

#include <herald/Object.h>

class QGraphicsItem;
class QSize;

namespace herald {

template <typename T>
class ScopedPtr;

class QtTextureTable;

/// The Qt interface for an object.
class QtObject : public Object {
public:
  /// Creates a new Qt object instance.
  /// @param parent A pointer to the parent graphics item.
  /// @returns A new Qt object instance.
  static ScopedPtr<QtObject> make(QGraphicsItem* parent);
  /// Just a stub.
  virtual ~QtObject() {}
  /// Adjusts the object size to account for
  /// a change in the standard size of an object.
  /// @param standard_size The new standard reference size.
  virtual void resize(const QSize& standard_size) = 0;
  /// Updates the position of the object.
  /// @param tile_size The size of a single tile, used for reference.
  virtual void update_position(const QSize& tile_size) = 0;
  /// Updates the texture used to display the object.
  /// @param textures The texture table to get the texture from.
  virtual void update_texture(const QtTextureTable& textures) = 0;
};

} // namespace herald
