#pragma once

#include <QToolBar>

namespace herald {

namespace tk {

/// Used for identifying which
/// tool was clicked from the tool panel.
enum class RoomToolID {
  /// Sets the background of the room.
  Background,
  /// Used for painting tiles.
  TileBrush
};

/// This widget is for showing
/// the available room tools for the user.
class RoomToolPanel : public QToolBar {
public:
  /// Constructs a new room tool panel instance.
  /// @param parent A pointer to the parent widget.
  RoomToolPanel(QWidget* parent);
signals:
  /// This signal is emitted when
  /// a tool selected from the panel.
  /// @param id The ID of the tool
  /// that was selected.
  void tool_selected(RoomToolID id);
};

} // namespace tk

} // namespace herald
