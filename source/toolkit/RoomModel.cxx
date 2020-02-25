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

std::size_t RoomModel::get_height() const noexcept {
  const auto* room = access_room();
  return room ? room->get_height() : 1;
}

std::size_t RoomModel::get_width() const noexcept {
  const auto* room = access_room();
  return room ? room->get_width() : 1;
}

bool RoomModel::is_valid() const noexcept {

  const auto* room_table = model->access_room_table();

  const auto* room = room_table->access_room(room_index);

  return room != nullptr;
}

const Room* RoomModel::access_room() const noexcept {
  return model->access_room_table()->access_room(room_index);
}

Room* RoomModel::modify_room() noexcept {
  return model->modify_room_table()->modify_room(room_index);
}

} // namespace tk

} // namespace herald
