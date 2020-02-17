#include <herald/ObjectTable.h>

#include <herald/Object.h>

namespace herald {

void ObjectTable::update_animation_indices(const ActionTable& actions) {
  for (std::size_t i = 0; i < size(); i++) {
    at(i)->update_animation_index(actions);
  }
}

} // namespace herald
