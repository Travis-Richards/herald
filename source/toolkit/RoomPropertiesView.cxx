#include "RoomPropertiesView.h"

#include <herald/ScopedPtr.h>

#include "Project.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QSpinBox>

namespace herald {

namespace tk {

namespace {

/// Used for viewing and modifying the room properties.
class RoomPropertiesViewImpl final : public RoomPropertiesView {
  /// The form layout of the room properties.
  ScopedPtr<QFormLayout> layout;
  /// The room width spin box.
  ScopedPtr<QSpinBox> w_spin_box;
  /// The room height spin box.
  ScopedPtr<QSpinBox> h_spin_box;
public:
  /// Constructs a new instance of the room properties view.
  /// @param parent A pointer to the parent widget.
  RoomPropertiesViewImpl(QWidget* parent) : RoomPropertiesView(parent){
    layout = ScopedPtr<QFormLayout>::make(this);
    setup();
  }
  /// Updates the data for the current room.
  void update_room_data(const Room* room) override {
    if (room) {
      w_spin_box->setValue(room->get_width());
      h_spin_box->setValue(room->get_height());
    }
  }
protected:
  /// Constructs the properties view.
  void setup() {

    w_spin_box = ScopedPtr<QSpinBox>::make(this);
    h_spin_box = ScopedPtr<QSpinBox>::make(this);

    w_spin_box->setEnabled(false);
    h_spin_box->setEnabled(false);

    w_spin_box->setMinimum(1);
    h_spin_box->setMinimum(1);

    // This is just a safe bet, since
    // the product of these two values
    // is less than or equal to the max
    // of a 32-bit integer.
    w_spin_box->setMaximum(0xffff);
    h_spin_box->setMaximum(0x7fff);

    layout->addRow(tr("Width"), w_spin_box.get());
    layout->addRow(tr("Height"), h_spin_box.get());

    using valueChanged = void(QSpinBox::*)(int);

    connect(w_spin_box.get(), static_cast<valueChanged>(&QSpinBox::valueChanged), this, &RoomPropertiesViewImpl::update_room_width);
    connect(h_spin_box.get(), static_cast<valueChanged>(&QSpinBox::valueChanged), this, &RoomPropertiesViewImpl::update_room_height);

    auto* check_box = new QCheckBox(this);

    check_box->setCheckState(Qt::Checked);

    connect(check_box, &QCheckBox::stateChanged, this, &RoomPropertiesViewImpl::on_size_lock_change);

    layout->addRow(tr("Size Lock"), check_box);
  }
  /// Sets the width of the room.
  /// @param width The width to assign the room.
  void update_room_width(int width) {
    emit width_changed((std::size_t) width);
  }
  /// Sets the height of the room.
  /// @param height The height to assign the room.
  void update_room_height(int height) {
    emit height_changed((std::size_t) height);
  }
  /// Handles the size lock being changed.
  /// @param state The new state of the size lock.
  void on_size_lock_change(int state) {
    auto is_enabled = (state == Qt::Unchecked);
    w_spin_box->setEnabled(is_enabled);
    h_spin_box->setEnabled(is_enabled);
  }
};

} // namespace

ScopedPtr<RoomPropertiesView> RoomPropertiesView::make(QWidget* parent) {
  return new RoomPropertiesViewImpl(parent);
}

} // namespace tk

} // namespace herald
