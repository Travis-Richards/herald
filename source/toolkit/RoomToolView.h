#pragma once

#include <QStackedWidget>

namespace herald {

namespace tk {

class RoomToolModel;

enum class RoomToolID : int;

/// Used to view/modify tool data.
class RoomToolView final : public QStackedWidget {
public:
  /// Constructs a new room tool view instance.
  /// @param model A model of the room tool data.
  /// @param parent A pointer to the parent widget.
  RoomToolView(RoomToolModel* model, QWidget* parent);
protected:
  /// Updates the tool view being being shown.
  /// @param tool_id The ID of the tool to show.
  void update_tool_view(RoomToolID tool_id);
};

} // namespace tk

} // namespace herald
