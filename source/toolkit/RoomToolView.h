#pragma once

#include <QStackedWidget>

namespace herald {

namespace tk {

class RoomToolMediator;

enum class RoomToolID : int;

/// Used to view/modify tool data.
class RoomToolView final : public QStackedWidget {
public:
  /// Constructs a new room tool view instance.
  /// @param mediator The mediator of the room tools.
  /// @param parent A pointer to the parent widget.
  RoomToolView(RoomToolMediator* mediator, QWidget* parent);
protected:
  /// Updates the tool view being being shown.
  /// @param tool_id The ID of the tool to show.
  void update_tool_view(RoomToolID tool_id);
};

} // namespace tk

} // namespace herald
