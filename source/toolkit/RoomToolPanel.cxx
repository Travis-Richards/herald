#include "RoomToolPanel.h"

#include "RoomToolMediator.h"

#include <QIcon>

namespace herald {

namespace tk {

RoomToolPanel::RoomToolPanel(RoomToolMediator* m, QWidget* parent) : QToolBar(parent), mediator(m) {

  setMovable(true);
  setFloatable(true);
  setOrientation(Qt::Vertical);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  connect(mediator, &RoomToolMediator::tool_changed, this, &RoomToolPanel::change_selected_icon);

  stamp = addAction(QIcon(":/icons/room-editor/stamp.svg"), tr("Tile Stamp"), [this]() {
    mediator->set_current_tool(RoomToolID::Stamp);
  });

  move = addAction(QIcon(":/icons/room-editor/move.svg"), tr("Move"), [this]() {
    mediator->set_current_tool(RoomToolID::Move);
  });

  rotate = addAction(QIcon(":/icons/room-editor/rotate.svg"), tr("Rotate"), [this]() {
    mediator->set_current_tool(RoomToolID::Rotate);
  });

  eraser = addAction(QIcon(":/icons/room-editor/eraser.svg"), tr("Eraser"), [this]() {
    mediator->set_current_tool(RoomToolID::Eraser);
  });

  eraser->setCheckable(true);
  move->setCheckable(true);
  rotate->setCheckable(true);
  stamp->setCheckable(true);
}

void RoomToolPanel::change_selected_icon(RoomToolID tool_id) {

  switch (tool_id) {
    case RoomToolID::None:
      eraser->setChecked(false);
      move->setChecked(false);
      rotate->setChecked(false);
      stamp->setChecked(false);
      break;
    case RoomToolID::Eraser:
      eraser->setChecked(true);
      move->setChecked(false);
      rotate->setChecked(false);
      stamp->setChecked(false);
      break;
    case RoomToolID::Move:
      eraser->setChecked(false);
      move->setChecked(true);
      rotate->setChecked(false);
      stamp->setChecked(false);
      break;
    case RoomToolID::Rotate:
      eraser->setChecked(false);
      move->setChecked(false);
      rotate->setChecked(true);
      stamp->setChecked(false);
      break;
    case RoomToolID::Stamp:
      eraser->setChecked(false);
      move->setChecked(false);
      rotate->setChecked(false);
      stamp->setChecked(true);
      break;
  }
}

} // namespace tk

} // namespace herald
