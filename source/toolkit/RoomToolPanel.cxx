#include "RoomToolPanel.h"

#include <QIcon>

namespace herald {

namespace tk {

#if 0
class ToolButton final : public QPushButton {
public:
  ToolButton(const QString& icon_path, QWidget* parent) : QPushButton(parent) {

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto* layout = new QHBoxLayout(this);

    layout->addWidget(make_label(icon_path, this));
  }
protected:
  static QWidget* make_label(const QString& icon_path, QWidget* parent) {
    QLabel* label = new QLabel(parent);
    label->setPixmap(QPixmap(icon_path));
    label->setScaledContents(true);
    return label;
  }
};
#endif

RoomToolPanel::RoomToolPanel(QWidget* parent) : QToolBar(parent) {

  setMovable(true);
  setFloatable(true);
  setOrientation(Qt::Vertical);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  addAction(QIcon(":/icons/room-editor/stamp.svg"), tr("Tile Brush"), [this]() {

  });

  addAction(QIcon(":/icons/room-editor/crop.svg"), tr("Set Background"), [this]() {

  });

  addAction(QIcon(":/icons/room-editor/move.svg"), tr("Move"), [this]() {

  });

  addAction(QIcon(":/icons/room-editor/rotate.svg"), tr("Rotate"), [this]() {

  });

  addAction(QIcon(":/icons/room-editor/eraser.svg"), tr("Eraser"), [this]() {

  });

#if 0
  auto* layout = new QVBoxLayout(this);
  layout->addWidget(new ToolButton(":/icons/room-editor/stamp.svg", this));
  layout->addWidget(new ToolButton(":/icons/room-editor/crop.svg",  this));
  layout->addWidget(new ToolButton(":/icons/room-editor/move.svg",  this));
  layout->addWidget(new ToolButton(":/icons/room-editor/rotate.svg",  this));
  layout->addWidget(new ToolButton(":/icons/room-editor/eraser.svg",  this));
#endif
}

} // namespace tk

} // namespace herald
