#include "Object.h"

#include "Action.h"

void Object::map_action(int index, const Action* action_obj) noexcept {
  if (action_obj) {
    action = index;
    animation = action_obj->get_animation();
    frame = 0;
    frame_offset = 0;
  } else {
    action = 0;
    animation = 0;
    frame = 0;
    frame_offset = 0;
  }
}
