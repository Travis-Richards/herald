#pragma once

#include <herald/TextureTable.h>

class QPixmap;

namespace herald {

template <typename T>
class ScopedPtr;

class Index;

/// The Qt interface of the texture table.
class QtTextureTable : public TextureTable {
public:
  /// Creates a new texture table.
  /// @returns A new texture table instance.
  static ScopedPtr<QtTextureTable> make();
  /// Just a stub.
  virtual ~QtTextureTable() {}
  /// Gets the pixmap for a texture at a specified index.
  /// @param index The index of the texture to access.
  /// @returns The pixmap for the specified texture.
  virtual QPixmap at(Index index) const = 0;
};

} // namespace herald
