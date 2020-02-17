#pragma once

#include <herald/Index.h>
#include <herald/Vec2f.h>

namespace herald {

class ActionTable;
class AnimationTable;

/// This is the base of any object.
class Object {
  /// The index of the action that the
  /// object is currently performing.
  Index action_index;
  /// The index of the animation that
  /// the object is currently performing.
  Index animation_index;
  /// The index of the texture that the
  /// item is currently displaying.
  Index texture_index;
  /// The position of the object.
  Vec2f position;
public:
  /// Accesses a pointer to a "null" object.
  /// Useful for returning an object in an
  /// out of bounds access situation.
  static Object* get_null_object() noexcept;
  /// Constructs a new object instance.
  constexpr Object() noexcept {}
  /// Just a stub.
  virtual ~Object() {}
  /// Updates the animation index.
  /// @param actions The action table to get the animation index from.
  virtual void update_animation_index(const ActionTable& actions);
  /// Updates the texture index for the object.
  /// @param ellapsed_ms The total number of ellapsed milliseconds.
  /// @param animations A reference to the animation table.
  virtual void update_texture_index(std::size_t ellapsed_ms, const AnimationTable& animations);
  /// Sets the action for the object to perform.
  /// @param index The index of the action for the object to perform.
  inline void set_action_index(Index index) noexcept {
    action_index = index;
  }
  /// Translates the object.
  /// @param delta_pos The change in position to apply.
  inline void translate(const Vec2f& delta_pos) noexcept {
    position = position + delta_pos;
  }
protected:
  /// Accesses the current texture index.
  inline Index get_texture_index() const noexcept {
    return texture_index;
  }
  /// Accesses the position of the object.
  const Vec2f& get_position() const noexcept {
    return position;
  }
};

} // namespace herald
