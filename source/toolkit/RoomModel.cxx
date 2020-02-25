#include "RoomModel.h"

#include "ProjectModel.h"

namespace herald {

namespace tk {

RoomModel::RoomModel(ProjectModel* m, QObject* parent) : QObject(parent) {
  model = m;
  room_index = SIZE_MAX;
}

bool RoomModel::change_room(std::size_t index) {

  if (index >= model->access_room_table()->size()) {
    return false;
  }

  room_index = index;

  emit room_changed();

  return true;
}

bool RoomModel::is_valid() const noexcept {

  const auto* room_table = model->access_room_table();

  const auto* room = room_table->access_room(room_index);

  return room != nullptr;
}

} // namespace tk

} // namespace herald
