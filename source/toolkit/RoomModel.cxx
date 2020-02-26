#include "RoomModel.h"

#include "Project.h"
#include "TileModel.h"

namespace herald {

namespace tk {

RoomModel::RoomModel(Project* m, QObject* parent) : QObject(parent) {
  project = m;
  room_index = SIZE_MAX;
}

bool RoomModel::change_room(std::size_t index) {

  if (index >= project->access_room_table()->size()) {
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

TileModel RoomModel::get_tile_model(std::size_t x, std::size_t y) {
  return TileModel(this, x, y);
}

std::size_t RoomModel::get_width() const noexcept {
  const auto* room = access_room();
  return room ? room->get_width() : 1;
}

bool RoomModel::is_valid() const noexcept {

  const auto* room_table = project->access_room_table();

  const auto* room = room_table->access_room(room_index);

  return room != nullptr;
}

bool RoomModel::set_width(std::size_t width) {

  if (has_width(width)) {
    return true;
  }

  auto* room = modify_room();
  if (room) {
    room->set_width(width);
    emit width_changed();
    return true;
  } else {
    return false;
  }
}

bool RoomModel::set_height(std::size_t height) {

  if (has_height(height)) {
    return true;
  }

  auto* room = modify_room();
  if (room) {
    room->set_height(height);
    emit height_changed();
    return true;
  } else {
    return false;
  }
}

const Room* RoomModel::access_room() const noexcept {
  return project->access_room_table()->access_room(room_index);
}

Room* RoomModel::modify_room() noexcept {
  return project->modify_room_table()->modify_room(room_index);
}

const Tile* RoomModel::access_tile(std::size_t x, std::size_t y) const noexcept {

  const auto* room = access_room();
  if (!room) {
    return nullptr;
  }

  return room->access_tile(x, y);
}

Tile* RoomModel::modify_tile(std::size_t x, std::size_t y) {

  auto* room = modify_room();
  if (!room) {
    return nullptr;
  }

  return room->modify_tile(x, y);
}

bool RoomModel::has_height(std::size_t h) const noexcept {

  const auto* room = access_room();

  return room ? (room->get_height() == h) : false;
}

bool RoomModel::has_width(std::size_t w) const noexcept {

  const auto* room = access_room();

  return room ? (room->get_width() == w) : false;
}

} // namespace tk

} // namespace herald
