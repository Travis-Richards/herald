#include "Room.h"

namespace {

/// Used as a placeholder until an actual room is built.
class NullRoom final : public Room {
public:
  /// Constructs an instance of the null room.
  /// @param parent A pointer to the parent object.
  NullRoom(QObject* parent) : Room(parent) {}
  /// Does nothing.
  /// @returns Always returns a null pointer.
  const Tile* get_tile(int, int) const noexcept override {
    return nullptr;
  }
  /// Does nothing.
  /// @returns Always returns zero.
  int width() const noexcept override {
    return 0;
  }
  /// Does nothing.
  /// @returns Always returns zero.
  int height() const noexcept override {
    return 0;
  }
  /// Does nothing.
  void update_tile_frames(int, int) noexcept override { }
};

} // namespace

Room* Room::make_null(QObject* parent) {
  return new NullRoom(parent);
}

Room::Room(QObject* parent) : QObject(parent) {

}
