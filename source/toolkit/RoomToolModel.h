#pragma once

#include <QObject>

namespace herald {

namespace tk {

/// Identifies the several types of room tools.
enum class RoomToolID : int {
  /// A placeholder for when no tool
  /// is currently selected.
  None,
  /// The eraser. This is used
  /// to remove content from a tile.
  Eraser,
  /// The move tool, used for rearranging tiles.
  Move,
  /// Used for rotating tiles.
  Rotate,
  /// The tile stamp. This is used
  /// to put artwork onto tiles.
  Stamp
};

/// This is the data model for the room tools.
class RoomToolModel final : public QObject {
  Q_OBJECT
public:
  /// Constructs a new room tool model instance.
  /// @param parent A pointer to the parent object.
  RoomToolModel(QObject* parent = nullptr) : QObject(parent), tool_id(RoomToolID::None) {}
  /// Sets the current room tool being used.
  /// @param id The ID of the tool to assign.
  void set_tool(RoomToolID id);
signals:
  /// This signal is emitted whenever a tool gets selected.
  /// @param id The ID of the selected tool.
  void tool_changed(RoomToolID id);
private:
  /// The ID of the currently used tool.
  RoomToolID tool_id;
};

} // namespace tk

} // namespace herald
