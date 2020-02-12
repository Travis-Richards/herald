#pragma once

/// Represents a single object within a room.
/// This is not related, in any way, to the idea
/// of an object in object oriented programming or
/// in the Qt object model.
class Object final {
  /// The action the object
  /// is currently performing.
  int action;
  /// The current animation
  /// the object is using.
  int animation;
  /// The current frame of
  /// the object animation.
  int frame;
  /// The starting frame offset
  /// of the object's animation.
  int frame_offset;
  /// The X coordinate of the object's location,
  /// in terms of tile location.
  float x;
  /// The Y coordinate of the object's location,
  /// in terms of tile location.
  float y;
public:
  /// Constructs the object.
  /// @param a The index of the action to assign the object.
  /// @param x_ The X coordinate of the object.
  /// @param y_ The Y coordinate of the object.
  Object(int a, int x_, int y_)
    : action(a),
      animation(0),
      frame(0),
      frame_offset(0),
      x(x_),
      y(y_) {}
  /// Accesses the index of the action
  /// this object is currently performing.
  inline int get_action() const noexcept {
    return action;
  }
  /// Accesses the index of the animation
  /// the object is currently performing.
  inline int get_animation() const noexcept {
    return animation;
  }
  /// Accesses the current frame of the object.
  inline int get_frame() const noexcept {
    return frame + frame_offset;
  }
  /// Accesses the X position of the object,
  /// in terms of tile location.
  inline float get_x() const noexcept {
    return x;
  }
  /// Accesses the Y position of the object,
  /// in terms of tile location.
  inline float get_y() const noexcept {
    return y;
  }
};
