#pragma once

#include "Index.h"

namespace herald {

class Index;

/// Represents a single, rectangular
/// tile used to make up a room's appearance.
class Tile {
  /// The index of the animation
  /// to render for this tile.
  Index animation_index;
public:
  /// Accesses a reference to a null tile.
  /// @returns A reference to a null tile.
  static Tile* get_null_tile() noexcept;
  /// Just a stub.
  virtual ~Tile() {}
  /// Queues an animation change.
  /// @param index The index of the animation to switch to.
  inline void set_animation_index(Index index) {
    animation_index = index;
  }
protected:
  /// Accesses the index of the animation
  /// for this tile.
  inline Index get_animation_index() const noexcept {
    return animation_index;
  }
};

} // namespace herald
