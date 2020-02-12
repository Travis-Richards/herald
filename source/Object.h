#pragma once

class Action;

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
  int x;
  /// The Y coordinate of the object's location,
  /// in terms of tile location.
  int y;
public:
  /// Constructs a default object.
  constexpr Object() noexcept
    : action(-1),
      animation(-1),
      frame(-1),
      frame_offset(0),
      x(0),
      y(0) {}
  /// Constructs the object.
  /// @param a The index of the action to assign the object.
  /// @param x_ The X coordinate of the object.
  /// @param y_ The Y coordinate of the object.
  constexpr Object(int a, int x_, int y_) noexcept
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
  inline int get_x() const noexcept {
    return x;
  }
  /// Accesses the Y position of the object,
  /// in terms of tile location.
  inline int get_y() const noexcept {
    return y;
  }
  /// Indicates whether or not an object owns an action.
  /// @param a The action to check for.
  /// @returns True if the actions match, false otherwise.
  inline bool has_action(int a) const noexcept {
    return action == a;
  }
  /// Indicates whether or not an object has a certain animation.
  /// @param a The animation to check for.
  /// @returns True if the object has the animation, false otherwise.
  inline bool has_animation(int a) const noexcept {
    return animation == a;
  }
  /// Maps an action onto this object.
  /// @param index The index of this action.
  /// @param action The action to map.
  /// This may be a null pointer.
  void map_action(int index, const Action* action) noexcept;
  /// Sets the new frame of the object.
  /// @param f The new frame to assign the object.
  void update_frame(int f) noexcept {
    frame = f;
  }
};
