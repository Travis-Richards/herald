#include "RoomEditor.h"

#include <herald/ScopedPtr.h>

#include "Project.h"
#include "RoomModel.h"
#include "RoomToolMediator.h"
#include "RoomToolPanel.h"
#include "RoomToolView.h"
#include "RoomView.h"
#include "TableButton.h"
#include "TableEditor.h"
#include "TableModel.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QTabWidget>

#include <climits>

namespace herald {

namespace tk {

namespace {

/// An implementation of the table model for the room table.
class RoomTableModel final : public TableModel {
  /// The project data to get the room table from.
  Project* project;
public:
  /// Constructs a new instance of the room table.
  /// @param m The model containing the room table.
  RoomTableModel(Project* m) : project(m) {
  }
  /// Creates a new room.
  /// @returns The name of the newly created room.
  QString create_room() {

    auto table_size = project->access_room_table()->size();

    auto last_index = index(table_size, 0);

    beginInsertRows(last_index, table_size, table_size + 1);

    auto name = project->modify_room_table()->create_room();

    endInsertRows();

    return name;
  }
  /// Gets the name of an item in the table.
  /// @param index The index of the item to get the name of.
  /// @returns the name of the item.
  QString get_name(std::size_t index) const override {
    return project->access_room_table()->get_name(index);
  }
  /// Gets the size of the room table.
  std::size_t get_size() const override {
    return project->access_room_table()->size();
  }
  /// Removes a room from the table.
  /// @param index The index of the room to remove.
  /// @returns True on success, false on failure.
  bool remove(std::size_t index) override {
    return project->modify_room_table()->remove(index);
  }
  /// Renames a room.
  /// @param index The index of the room to rename.
  /// @param name The name to assign the item.
  /// @returns True on success, false on failure.
  bool rename(std::size_t index, const QString& name) override {
    return project->modify_room_table()->rename(index, name);
  }
};

/// Used for viewing and modifying the room properties.
class RoomPropertiesView final : public QWidget {
  /// A pointer to the room model.
  RoomModel* model;
  /// The form layout of the room properties.
  ScopedPtr<QFormLayout> layout;
  /// The room width spin box.
  ScopedPtr<QSpinBox> w_spin_box;
  /// The room height spin box.
  ScopedPtr<QSpinBox> h_spin_box;
public:
  /// Constructs a new instance of the room properties view.
  /// @param m A pointer to the room data model.
  /// @param parent A pointer to the parent widget.
  RoomPropertiesView(RoomModel* m, QWidget* parent) : QWidget(parent), model(m) {

    layout = ScopedPtr<QFormLayout>::make(this);

    setup();

    connect(model, &RoomModel::room_changed, this, &RoomPropertiesView::update_room);
  }
protected:
  /// Constructs the properties view.
  void setup() {

    w_spin_box = ScopedPtr<QSpinBox>::make(this);
    h_spin_box = ScopedPtr<QSpinBox>::make(this);

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

    connect(w_spin_box.get(), static_cast<valueChanged>(&QSpinBox::valueChanged), this, &RoomPropertiesView::update_room_width);
    connect(h_spin_box.get(), static_cast<valueChanged>(&QSpinBox::valueChanged), this, &RoomPropertiesView::update_room_height);
  }
  /// Updates the data for the current room.
  void update_room() {
    w_spin_box->setValue(model->get_width());
    h_spin_box->setValue(model->get_height());
  }
  /// Sets the width of the room.
  /// @param width The width to assign the room.
  void update_room_width(int width) {
    model->set_width((std::size_t) width);
  }
  /// Sets the height of the room.
  /// @param height The height to assign the room.
  void update_room_height(int height) {
    model->set_height((std::size_t) height);
  }
};

/// Used for controlling the currently selected tool.
class ToolControl final : public QTabWidget {
public:
  /// Constructs a new instance of the tool control widget.
  /// @param room_model A pointer to the room model.
  /// @param room_tool_mediator A pointer to the room tool model.
  /// @param parent A pointer to the parent widget.
  ToolControl(RoomModel* room_model,
              RoomToolMediator* room_tool_mediator,
              QWidget* parent) : QTabWidget(parent) {

    addTab(new RoomPropertiesView(room_model, this), tr("Room Properties"));
    addTab(new RoomToolView(room_tool_mediator, this), tr("Tool Properties"));

    connect(room_tool_mediator, &RoomToolMediator::tool_changed, this, &ToolControl::on_tool_changed);
  }
protected:
  /// Handles a tool being changed by
  /// switching over to the tool properties widget.
  void on_tool_changed(RoomToolID) {
    setCurrentIndex(1);
  }
};

/// A widget used for editing rooms.
class RoomEditor final : public QWidget {
  /// Identifies the "New Room" button.
  static constexpr std::size_t new_room_button_id = 0;
  /// The data model for the room tools.
  ScopedPtr<RoomToolMediator> room_tool_mediator;
  /// The model for the room table.
  ScopedPtr<RoomTableModel> room_table_model;
  /// The room table editor.
  ScopedPtr<TableEditor> room_table_editor;
  /// The data model of the room being viewed.
  ScopedPtr<RoomModel> room_model;
  /// A view of the room being edited.
  ScopedPtr<RoomView> room_view;
  /// The tool panel for the room editor.
  ScopedPtr<RoomToolPanel> tool_panel;
  /// The widget used for controlling tool behavior.
  ScopedPtr<ToolControl> tool_control;
public:
  /// Constructs a new room editor instance.
  /// @param parent A pointer to the parent widget.
  RoomEditor(Project* m, QWidget* parent) : QWidget(parent) {

    room_tool_mediator = RoomToolMediator::make(m, this);

    room_table_model = ScopedPtr<RoomTableModel>::make(m);

    room_table_editor = TableEditor::make(room_table_model.get(), this);
    room_table_editor->add_button(new_room_button_id, tr("New Room"));

    room_model = ScopedPtr<RoomModel>::make(m, this);

    room_view = RoomView::make(room_model.get(), room_tool_mediator.get(), this);

    tool_panel = ScopedPtr<RoomToolPanel>::make(room_tool_mediator.get(), this);

    tool_control = ScopedPtr<ToolControl>::make(room_model.get(), room_tool_mediator.get(), this);

    connect(room_table_editor.get(), &TableEditor::button_clicked, this, &RoomEditor::on_table_button);
    connect(room_table_editor.get(), &TableEditor::selected,       this, &RoomEditor::on_room_selected);

    auto* layout = new QGridLayout(this);
    layout->addWidget(room_table_editor->get_widget(), 0,  0, 1,  4);
    layout->addWidget(tool_control.get(),              1,  0, 1,  4);
    layout->addWidget(room_view->get_widget(),         0,  4, 2, 15);
    layout->addWidget(tool_panel.get(),                0, 19, 2,  1);
  }
protected:
  /// Handles a table button being clicked.
  /// @param id The ID of the button that was clicked.
  void on_table_button(const TableButtonID& button_id) {
    if (button_id.is_predefined()) {
      if (button_id.has_id(TableEditor::remove_button_id)) {
        // Remove
      }
    } else if (button_id.has_id(new_room_button_id)) {
      room_table_model->create_room();
    }
  }
  /// This function is called when a room is selected from the room table.
  /// @param index The index of the room that was selected.
  void on_room_selected(std::size_t index) {
    room_model->change_room(index);
  }
};

} // namespace

ScopedPtr<QWidget> make_room_editor(Project* project, QWidget* parent) {
  return new RoomEditor(project, parent);
}

} // namespace tk

} // namespace herald
