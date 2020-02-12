#pragma once

#include <QObject>

/// Represents a single object within a room.
/// This is not related, in any way, to the idea
/// of an object in object oriented programming or
/// in the Qt object model.
class Object : public QObject {
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
  /// Constructs the base object.
  /// @param parent A pointer to the parent Qt object.
  Object(int a, int x_, int y_, QObject* parent)
    : QObject(parent),
      action(a),
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
