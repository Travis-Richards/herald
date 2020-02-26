#include "RoomToolPanel.h"

#include "RoomToolModel.h"

#include <QIcon>

namespace herald {

namespace tk {

RoomToolPanel::RoomToolPanel(RoomToolModel* m, QWidget* parent) : QToolBar(parent), model(m) {

  setMovable(true);
  setFloatable(true);
  setOrientation(Qt::Vertical);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  connect(model, &RoomToolModel::tool_changed, this, &RoomToolPanel::change_selected_icon);

  stamp = addAction(QIcon(":/icons/room-editor/stamp.svg"), tr("Tile Stamp"), [this]() {
    model->set_tool(RoomToolID::Stamp);
  });

  addAction(QIcon(":/icons/room-editor/crop.svg"), tr("Set Background"), [this]() {
  });

  move = addAction(QIcon(":/icons/room-editor/move.svg"), tr("Move"), [this]() {
    model->set_tool(RoomToolID::Move);
  });

  rotate = addAction(QIcon(":/icons/room-editor/rotate.svg"), tr("Rotate"), [this]() {
    model->set_tool(RoomToolID::Rotate);
  });

  eraser = addAction(QIcon(":/icons/room-editor/eraser.svg"), tr("Eraser"), [this]() {
    model->set_tool(RoomToolID::Eraser);
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
