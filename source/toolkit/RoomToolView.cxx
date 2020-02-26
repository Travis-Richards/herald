#include "RoomToolView.h"

#include "RoomToolModel.h"

#include <herald/ScopedPtr.h>

#include <QComboBox>
#include <QFormLayout>

namespace herald {

namespace tk {

namespace {

/// This is the widget that gets displayed when
/// there's no tool currently selected.
class PlaceholderView final : public QWidget {
public:
  /// Constructs a new placeholder view.
  /// @param parent A pointer to the parent widget.
  PlaceholderView(QWidget* parent) : QWidget(parent) {
  }
};

/// A view of the stamp tool settings.
class StampToolView final : public QWidget {
  /// The layout of the stamp tool.
  QFormLayout layout;
  /// A pointer to the room tool data model.
  RoomToolModel* tool_model;
  /// The combo box for selecting a texture.
  ScopedPtr<QComboBox> texture_combo_box;
public:
  /// Constructs a new instance of the stamp tool view.
  /// @param m A pointer to the room tool data model.
  /// @param parent A pointer to the parent widget.
  StampToolView(RoomToolModel* m, QWidget* parent) : QWidget(parent), layout(this), tool_model(m) {

    texture_combo_box = ScopedPtr<QComboBox>::make(this);

    layout.addRow(tr("Texture"), texture_combo_box.get());

    fill_texture_options();
  }
protected:
  /// Fills the texture combo box with the texture options.
  void fill_texture_options() {

    auto* stamp_tool = tool_model->get_stamp_tool();

    auto textures = stamp_tool->list_textures();

    for (auto texture : textures) {
      texture_combo_box->addItem(texture);
    }
  }
};

} // namespace

RoomToolView::RoomToolView(RoomToolModel* model, QWidget* parent) : QStackedWidget(parent) {

  // placeholder
  addWidget(new PlaceholderView(this));

  // eraser
  addWidget(new PlaceholderView(this));

  // move
  addWidget(new PlaceholderView(this));

  // rotate
  addWidget(new PlaceholderView(this));

  // stamp
  addWidget(new StampToolView(model, this));

  setCurrentIndex(0);

  connect(model, &RoomToolModel::tool_changed, this, &RoomToolView::update_tool_view);
}

void RoomToolView::update_tool_view(RoomToolID tool_id) {
  switch (tool_id) {
    case RoomToolID::None:
      setCurrentIndex(0);
      break;
    case RoomToolID::Eraser:
      setCurrentIndex(1);
      break;
    case RoomToolID::Move:
      setCurrentIndex(2);
      break;
    case RoomToolID::Rotate:
      setCurrentIndex(3);
      break;
    case RoomToolID::Stamp:
      setCurrentIndex(4);
      break;
  }
}

} // namespace tk

} // namespace herald
