#pragma once

#include <QToolBar>

namespace herald {

namespace tk {

class RoomToolMediator;

enum class RoomToolID : int;

/// This widget is for showing
/// the available room tools for the user.
class RoomToolPanel : public QToolBar {
public:
  /// Constructs a new room tool panel instance.
  /// @param mediator A pointer to the room tool data model.
  /// @param parent A pointer to the parent widget.
  RoomToolPanel(RoomToolMediator* mediator, QWidget* parent);
protected:
  /// Changes the selected item from the tool bar.
  /// @param tool_id The ID of the tool that was selected.
  void change_selected_icon(RoomToolID tool_id);
private:
  /// The mediator for the room tools.
  RoomToolMediator* mediator;
  /// The eraser action.
  QAction* eraser;
  /// The tile moving action.
  QAction* move;
  /// The tile rotation action.
  QAction* rotate;
  /// The stab action.
  QAction* stamp;
};

} // namespace tk

} // namespace herald
