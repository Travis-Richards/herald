#pragma once

#include <cstddef>

namespace herald {

template <typename T>
class ScopedPtr;

class Index;

/// The interface for an animation.
class Animation {
public:
  /// Accesses a pointer to the null animation.
  /// @returns A pointer to the null animation instance.
  static Animation* get_null_animation() noexcept;
  /// Creates a new animation instance.
  /// @returns A new animation instance.
  static ScopedPtr<Animation> make();
  /// Creates a single frame animation.
  /// This is a convenience function for making an animation with just one frame.
  /// @param texture The texture to display for the animation.
  static ScopedPtr<Animation> make_single_frame(Index texture);
  /// Just a stub.
  virtual ~Animation() {}
  /// Adds a frame to the animation sequence.
  /// @param texture The index of the texture to add.
  /// @param duration The duration of the frame, in terms of milliseconds.
  virtual void add_frame(Index texture, std::size_t duration) = 0;
  /// Calculates the texture index for a certain point in time.
  /// @param ellapsed_ms The point in time to get the texture index for.
  /// @returns The texture index at the specified point in time.
  virtual Index calculate_texture_index(std::size_t ellapsed_ms) const noexcept = 0;
};

} // namespace herald
