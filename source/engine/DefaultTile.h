#pragma once

#include "Tile.h"

namespace herald {

template <typename T>
class ScopedPtr;

/// The default implementation of the tile.
class DefaultTile : public Tile {
public:
  /// Creates a new default tile instance.
  static ScopedPtr<DefaultTile> make();
  /// Just a stub.
  virtual ~DefaultTile() {}
};

} // namespace herald
