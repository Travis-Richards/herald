#pragma once

#include "Index.h"

namespace herald {

/// The background animation.
class Background {
  /// The index of the animation used
  /// for the background display.
  Index animation_index;
public:
  /// Just a stub.
  virtual ~Background() {}
  /// Assigns the animation index of the background.
  /// @param index The index of the animation to be displayed.
  inline void set_animation_index(Index index) noexcept {
    animation_index = index;
  }
};

} // namespace herald
