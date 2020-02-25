#pragma once

#include <QObject>

namespace herald {

namespace tk {

class ProjectModel;

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
  /// Indicates if the room model is valid or not
  /// The room valid may not be valid if it does
  /// not point to a particular room. This may be
  /// the case if the user has not yet selected a room
  bool is_valid() const noexcept;
signals:
  /// This signal is emitted when the
  /// room that is being modeled is changed.
  void room_changed();
private:
  /// The project model to get the room information from.
  ProjectModel* model;
  /// The index of the room within the room table.
  std::size_t room_index;
};

} // namespace tk

} // namespace herald
