#pragma once

/// Contains the data used by the
/// background of the current room.
class Background final {
  /// The index of the animation that
  /// the background is displaying.
  int animation;
  /// The frame that the background
  /// is currently at.
  int frame;
  /// The starting frame offset
  /// of the background animation.
  int frame_offset;
public:
  /// Constructs a background instance.
  constexpr Background() noexcept
    : animation(0), frame(0), frame_offset(0) {}
  /// Accesses the animation index
  /// of the background.
  inline int get_animation() const noexcept {
    return animation;
  }
  /// Accesses the current frame of
  /// the background animation.
  inline int get_frame() const noexcept {
    return frame_offset + frame;
  }
  /// Assigns the animation of the background.
  /// @param a The animation index to assign.
  inline void set_animation(int a) noexcept {
    animation = a;
  }
  /// Assigns the current frame
  /// of the background.
  inline void set_frame(int f) noexcept {
    frame = f;
  }
  /// Assigns the frame offset of the background.
  /// @param offset The fram offset to assign.
  inline void set_frame_offset(int offset) noexcept {
    frame_offset = offset;
  }
};
