#pragma once

#include <QObject>

namespace herald {

namespace tk {

class ProjectModel;
class Room;

/// This is a wrapper around the room class
/// so that a single room can fit into the
/// model/view paradigm.
class RoomModel final : public QObject {
  Q_OBJECT
public:
  /// Constructs a new instance of the room model.
  /// @param model A pointer to the project model
  /// to get the room information from.
  RoomModel(ProjectModel* model, QObject* parent);
  /// Changes the room being modeled.
  /// @param index The index of the room to change to.
  /// @returns True on success, false on failure.
  /// This function may fail if @p index is out of bounds.
  bool change_room(std::size_t index);
  /// Gets the height of the current room, in terms of tiles.
  std::size_t get_height() const noexcept;
  /// Gets the width of the current room, in terms of tiles.
  std::size_t get_width() const noexcept;
  /// Indicates if the room model is valid or not
  /// The room valid may not be valid if it does
  /// not point to a particular room. This may be
  /// the case if the user has not yet selected a room
  bool is_valid() const noexcept;
  /// Sets the width of the room.
  /// @param width The width to assign the room,
  /// in terms of tiles.
  /// @returns True on success, false on failure.
  bool set_width(std::size_t width);
  /// Sets the height of the room.
  /// @param The height to assign the room,
  /// in terms of tiles.
  /// @returns True on success, false on failure.
  bool set_height(std::size_t height);
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
protected:
  /// Gets a pointer to the current room.
  /// @returns A const-pointer to the current room.
  /// If no room is currently selected, then a null point is returned.
  const Room* access_room() const noexcept;
  /// Gets a pointer to current room, for modification.
  /// This function has the side effect of setting the
  /// project's modification flag to true.
  /// @returns A const-pointer to the current room.
  /// If no room is currently selected, then a null point is returned.
  Room* modify_room() noexcept;
  /// Indicates if a room has a certain height value.
  bool has_height(std::size_t h) const noexcept;
  /// Indicates if a room has a certain width value.
  bool has_width(std::size_t w) const noexcept;
private:
  /// The project model to get the room information from.
  ProjectModel* model;
  /// The index of the room within the room table.
  std::size_t room_index;
};

} // namespace tk

} // namespace herald
