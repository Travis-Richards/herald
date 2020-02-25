#include "RoomEditor.h"

#include <herald/ScopedPtr.h>

#include "ProjectModel.h"
#include "RoomModel.h"
#include "RoomToolPanel.h"
#include "RoomView.h"
#include "TableButton.h"
#include "TableEditor.h"
#include "TableModel.h"

#include <QGridLayout>

namespace herald {

namespace tk {

namespace {

/// An implementation of the table model for the room table.
class RoomTableModel final : public TableModel {
  /// The project model to get the room table from.
  ProjectModel* model;
public:
  /// Constructs a new instance of the room table.
  /// @param m The model containing the room table.
  RoomTableModel(ProjectModel* m) : model(m) {
  }
  /// Creates a new room.
  /// @returns The name of the newly created room.
  QString create_room() {

    auto table_size = model->access_room_table()->size();

    auto last_index = index(table_size, 0);

    beginInsertRows(last_index, table_size, table_size + 1);

    auto name = model->modify_room_table()->create_room();

    endInsertRows();

    return name;
  }
  /// Gets the name of an item in the table.
  /// @param index The index of the item to get the name of.
  /// @returns the name of the item.
  QString get_name(std::size_t index) const override {
    return model->access_room_table()->get_name(index);
  }
  /// Gets the size of the room table.
  std::size_t get_size() const override {
    return model->access_room_table()->size();
  }
  /// Removes a room from the table.
  /// @param index The index of the room to remove.
  /// @returns True on success, false on failure.
  bool remove(std::size_t index) override {
    return model->modify_room_table()->remove(index);
  }
  /// Renames a room.
  /// @param index The index of the room to rename.
  /// @param name The name to assign the item.
  /// @returns True on success, false on failure.
  bool rename(std::size_t index, const QString& name) override {
    return model->modify_room_table()->rename(index, name);
  }
};

/// A widget used for editing rooms.
class RoomEditor final : public QWidget {
  /// Identifies the "New Room" button.
  static constexpr std::size_t new_room_button_id = 0;
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
public:
  /// Constructs a new room editor instance.
  /// @param parent A pointer to the parent widget.
  RoomEditor(ProjectModel* m, QWidget* parent) : QWidget(parent) {

    room_table_model = ScopedPtr<RoomTableModel>::make(m);

    room_table_editor = TableEditor::make(room_table_model.get(), this);
    room_table_editor->add_button(new_room_button_id, tr("New Room"));

    room_model = ScopedPtr<RoomModel>::make(m, this);

    room_view = RoomView::make(room_model.get(), this);

    tool_panel = ScopedPtr<RoomToolPanel>::make(this);

    connect(room_table_editor.get(), &TableEditor::button_clicked, this, &RoomEditor::on_table_button);
    connect(room_table_editor.get(), &TableEditor::selected,       this, &RoomEditor::on_room_selected);

    auto* layout = new QGridLayout(this);
    layout->addWidget(room_table_editor->get_widget(), 0,  0, 0, 4);
    layout->addWidget(room_view->get_widget(),         0,  4, 0, 15);
    layout->addWidget(tool_panel.get(),                0, 19, 0, 1);
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

ScopedPtr<QWidget> make_room_editor(ProjectModel* model, QWidget* parent) {
  return new RoomEditor(model, parent);
}

} // namespace tk

} // namespace herald
