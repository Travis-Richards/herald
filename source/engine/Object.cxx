#include <herald/Object.h>

#include <herald/Action.h>
#include <herald/ActionTable.h>
#include <herald/Animation.h>
#include <herald/AnimationTable.h>

namespace herald {

namespace {

class NullObject final : public Object {
public:
};

/// A null object for returning
/// an object in an out of bounds
/// access situation.
NullObject null_object;

} // namespace

Object* Object::get_null_object() noexcept {
  return &null_object;
}

void Object::update_animation_index(const ActionTable& actions) {
  auto* action = actions.at(action_index);
  if (action) {
    animation_index = action->get_animation_index();
  }
}

void Object::update_texture_index(std::size_t ellapsed_ms, const AnimationTable& animations) {

  auto* animation = animations.at(animation_index);
  if (animation) {
    texture_index = animation->calculate_texture_index(ellapsed_ms);
  }
}

} // namespace herald
