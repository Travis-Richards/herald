#pragma once

#include <QObject>

namespace herald {

namespace tk {

class Project;
class Room;
class Tile;

/// A mediator to the currently selected
/// room in the room editor.
class RoomMediator final : public QObject {
  Q_OBJECT
public:
  /// Constructs a new instance of the room model.
  /// @param project A pointer to the project data.
  /// to get the room information from.
  RoomMediator(Project* project, QObject* parent);
  /// Gets a pointer to the current room.
  /// @returns A const-pointer to the current room.
  /// If no room is currently selected, then a null point is returned.
  const Room* access_room() const noexcept;
  /// Changes the room being modeled.
  /// @param index The index of the room to change to.
  /// @returns True on success, false on failure.
  /// This function may fail if @p index is out of bounds.
  bool change_room(std::size_t index);
  /// Indicates if the room model is valid or not
  /// The room valid may not be valid if it does
  /// not point to a particular room. This may be
  /// the case if the user has not yet selected a room
  bool is_valid() const noexcept;
  /// Gets a pointer to current room, for modification.
  /// This function has the side effect of setting the
  /// project's modification flag to true.
  /// @returns A const-pointer to the current room.
  /// If no room is currently selected, then a null point is returned.
  Room* modify_room() noexcept;
signals:
  /// This signal is emitted when the
  /// room that is being modeled is changed.
  void room_changed();
  /// This signal is emitted when the
  /// room width is modified.
  void width_changed();
  /// This signal is emitted when the
  /// room height is modified.
  void height_changed();
private:
  /// The project data to get the room information from.
  Project* project;
  /// The index of the room within the room table.
  std::size_t room_index;
};

} // namespace tk

} // namespace herald
