#include "RoomMediator.h"

#include "Project.h"

namespace herald {

namespace tk {

RoomMediator::RoomMediator(Project* m, QObject* parent) : QObject(parent) {
  project = m;
  room_index = SIZE_MAX;
}

const Room* RoomMediator::access_room() const noexcept {
  return project->access_room_table()->access_room(room_index);
}

bool RoomMediator::change_room(std::size_t index) {

  if (index >= project->access_room_table()->size()) {
    return false;
  }

  room_index = index;

  emit room_changed();

  return true;
}

bool RoomMediator::is_valid() const noexcept {

  const auto* room_table = project->access_room_table();

  const auto* room = room_table->access_room(room_index);

  return room != nullptr;
}

Room* RoomMediator::modify_room() noexcept {
  return project->modify_room_table()->modify_room(room_index);
}

} // namespace tk

} // namespace herald
