#pragma once

#include "Index.h"
#include "Vec2f.h"

namespace herald {

/// Represents an arbitrary action
/// that an object can do.
class Action final {
  /// The index of the animation
  /// to render for this action.
  Index animation;
public:
  /// The default action constructor.
  constexpr Action() noexcept {}
  /// Constructs an initialized action instance.
  /// @param a The animation to assign the action.
  constexpr Action(Index a) noexcept : animation(a) {}
  /// Accesses the animation index of the action.
  /// @returns The animation index of the action.
  inline constexpr Index get_animation_index() const noexcept {
    return animation;
  }
};

} // namespace herald
