#include "RoomEditor.h"

#include <herald/ScopedPtr.h>

#include "Project.h"
#include "RoomMediator.h"
#include "RoomPropertiesView.h"
#include "RoomToolMediator.h"
#include "RoomToolPanel.h"
#include "RoomToolView.h"
#include "RoomView.h"
#include "TableButton.h"
#include "TableEditor.h"
#include "TableModel.h"

#include <QGridLayout>
#include <QTabWidget>

#include <vector>

#include <climits>

namespace herald {

namespace tk {

namespace {

/// Modifies a tile based on the current room tool.
class TileModifier final : public RoomToolVisitor {
  /// A pointer to the tile to be modified.
  Tile* tile;
  /// A pointer to the tile view to be modified.
  TileView* tile_view;
public:
  /// Constructs a new instance of the tool modifier.
  /// @param t A pointer to the tile to be modified.
  /// @param tv A view of the tile view to modify.
  TileModifier(Tile* t, TileView* tv) : tile(t), tile_view(tv) {}
protected:
  /// Erases a texture from a tile.
  void visit(const EraserTool&) override {
    tile->set_texture("");
    tile_view->load_texture_data(QByteArray());
  }
  /// Applies the stamp tool to the tile.
  /// @param stamp_tool The stamp tool to get the data from.
  void visit(const StampTool& stamp_tool) override {

    tile->set_texture(stamp_tool.get_texture_name());

    tile_view->load_texture_data(stamp_tool.get_texture_data());
  }
};

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

/// This is a single tab for an opened room.
class OpenedRoom final {
  /// The room being modified.
  Room* room;
  /// A view of the room.
  ScopedPtr<RoomView> view;
public:
  /// Constructs a new opened room instance.
  /// @param r The room that's opened.
  /// @param v A view of the room.
  OpenedRoom(Room* r, ScopedPtr<RoomView>&& v) : room(r), view(v) { }
  /// Constructs an opened room via move semantics.
  /// @param other The opened room instance to move the data from.
  OpenedRoom(OpenedRoom&& other) : room(other.room), view(std::move(other.view)) { }
  /// Gets the name of the opened room.
  /// @returns The name of the opened room.
  QString get_name() const {
    return room->get_name();
  }
  /// Gets a pointer to the root widget.
  QWidget* get_widget() noexcept {
    return view.get();
  }
  /// Gets a pointer to the opened room.
  Room* get_room() noexcept {
    return room;
  }
  /// Gets a pointer to the room view.
  RoomView* get_view() noexcept {
    return view.get();
  }
  /// Indicates if this room has a certain name.
  /// @param name The name to check for.
  /// @returns True if this room has the specified name, false otherwise.
  bool has_name(const QString& name) const {
    return room->get_name() == name;
  }
};

/// Manages all opened rooms.
class OpenedRoomManager final : public QTabWidget {
  /// The list of opened rooms.
  std::vector<OpenedRoom> opened_rooms;
public:
  /// Constructs a new instance of the opened room manager.
  /// @param parent A pointer to the parent widget.
  OpenedRoomManager(QWidget* parent) : QTabWidget(parent) { }
  /// Adds a room view to the opened room manager.
  /// @param room_view The room view to add.
  void add(OpenedRoom&& room) {

    addTab(room.get_widget(), room.get_name());

    opened_rooms.emplace_back(std::move(room));
  }
  /// Accesses a pointer to the currently opened room.
  /// @returns A pointer to the currently opened room.
  OpenedRoom* get_current() {

    auto index = currentIndex();
    if (index < 0) {
      return nullptr;
    }

    return &opened_rooms[(std::size_t) index];
  }
  /// Switches to an existing opened room, if it is actually open.
  /// @param room The room to switch to.
  /// @returns True on success, false on failure.
  /// This function will fail if the room hasn't been opened.
  bool open_existing(const Room* room) {

    for (std::size_t i = 0; i < opened_rooms.size(); i++) {
      if (opened_rooms[i].has_name(room->get_name())) {
        setCurrentIndex((int) i);
        return true;
      }
    }

    return false;
  }
};

/// A widget used for editing rooms.
class RoomEditor final : public QWidget {
  /// Identifies the "New Room" button.
  static constexpr std::size_t new_room_button_id = 0;
  /// A pointer to the project data.
  Project* project;
  /// The data model for the room tools.
  ScopedPtr<RoomToolMediator> room_tool_mediator;
  /// The model for the room table.
  ScopedPtr<RoomTableModel> room_table_model;
  /// The room table editor.
  ScopedPtr<TableEditor> room_table_editor;
  /// The data model of the room being viewed.
  ScopedPtr<RoomMediator> room_mediator;
  /// Contains all of the opened rooms.
  ScopedPtr<OpenedRoomManager> opened_room_manager;
  /// The tool panel for the room editor.
  ScopedPtr<RoomToolPanel> tool_panel;
  /// The control panel for the room properties.
  RoomPropertiesView* room_properties_view;
  /// The widget used for controlling tool behavior.
  ScopedPtr<QTabWidget> tool_control;
public:
  /// Constructs a new room editor instance.
  /// @param parent A pointer to the parent widget.
  RoomEditor(Project* p, QWidget* parent) : QWidget(parent), project(p) {

    room_tool_mediator = RoomToolMediator::make(project, this);

    room_table_model = ScopedPtr<RoomTableModel>::make(project);

    room_table_editor = TableEditor::make(room_table_model.get(), this);
    room_table_editor->add_button(new_room_button_id, tr("New Room"));

    room_mediator = ScopedPtr<RoomMediator>::make(project, this);

    opened_room_manager = ScopedPtr<OpenedRoomManager>::make(this);

    tool_panel = ScopedPtr<RoomToolPanel>::make(room_tool_mediator.get(), this);

    tool_control = ScopedPtr<QTabWidget>::make(this);

    room_properties_view = RoomPropertiesView::make(tool_control.get()).release();

    auto* room_tool_view = new RoomToolView(room_tool_mediator.get(), tool_control.get());

    tool_control->addTab(room_properties_view, tr("Room Properties"));
    tool_control->addTab(room_tool_view, tr("Tool Properties"));

    connect(room_properties_view, &RoomPropertiesView::height_changed, this, &RoomEditor::update_room_height);
    connect(room_properties_view, &RoomPropertiesView::width_changed,  this, &RoomEditor::update_room_width);

    connect(room_table_editor.get(), &TableEditor::button_clicked, this, &RoomEditor::on_table_button);
    connect(room_table_editor.get(), &TableEditor::selected,       this, &RoomEditor::on_room_selected);

    auto* layout = new QGridLayout(this);
    layout->addWidget(room_table_editor->get_widget(), 0,  0, 1,  4);
    layout->addWidget(tool_control.get(),              1,  0, 1,  4);
    layout->addWidget(opened_room_manager.get(),       0,  4, 2, 15);
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

    auto* room_table = project->modify_room_table();
    if (!room_table) {
      return;
    }

    auto* room = room_table->modify_room(index);
    if (!room) {
      return;
    }

    room_mediator->change_room(index);

    if (!opened_room_manager->open_existing(room)) {
      open_room(room);
    }
  }
  /// Opens a room for editing.
  /// @param room A pointer to the room to open.
  /// @returns True on success, false on failure.
  bool open_room(Room* room) {

    room_properties_view->update_room_data(room);

    auto room_view = RoomView::make(this);

    create_rows(room_view.get(), room, room->get_height());

    opened_room_manager->add(OpenedRoom(room, std::move(room_view)));

    return true;
  }
  /// Creates a new tile view instance.
  /// @param parent A pointer to the parent widget.
  /// @returns A pointer to the new tile view instance.
  ScopedPtr<TileView> make_tile_view(QWidget* parent) {

    auto tile_view = TileView::make(parent);

    auto mod_functor = [this](TileView* tile_view) { modify_tile(tile_view); };

    connect(tile_view.get(), &TileView::clicked, mod_functor);

    return tile_view;
  }
  /// Modifies a clicked tile, based on
  /// the currently selected tool.
  /// @param tile_view A pointer to the tile view that was clicked.
  void modify_tile(TileView* tile_view) {

    auto* opened_room = opened_room_manager->get_current();
    if (!opened_room) {
      return;
    }

    auto* view = opened_room->get_view();

    std::size_t x = 0;
    std::size_t y = 0;

    if (!view->find_location(tile_view, x, y)) {
      return;
    }

    auto* room = opened_room->get_room();

    auto* tile = room->modify_tile(x, y);
    if (!tile) {
      return;
    }

    auto* room_tool = room_tool_mediator->get_current_tool();

    TileModifier tile_modifier(tile, tile_view);

    room_tool->accept(tile_modifier);
  }
  /// Updates the height of the currently opened room.
  /// @param height The height to adjust the current room to.
  void update_room_height(std::size_t height) {

    auto* opened_room = opened_room_manager->get_current();
    if (!opened_room) {
      return;
    }

    auto* room = opened_room->get_room();

    auto original_height = room->get_height();

    room->set_height(height);

    if (original_height > height) {
      remove_rows(opened_room->get_view(), original_height - height);
    } else {
      create_rows(opened_room->get_view(), room, height - original_height);
    }
  }
  /// Removes rows from the room view.
  /// @param view The view to remove rows from.
  /// @param count The number of rows to remove.
  void remove_rows(RoomView* view, std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {
      view->remove_last_row();
    }
  }
  /// Creates rows for the room view.
  /// @param view The view to put the rows into.
  /// @param room The room to make the rows for.
  /// @param count The number of rows to make.
  void create_rows(RoomView* view, const Room* room, std::size_t count) {

    const auto* textures = project->access_texture_table();

    auto y_initial = view->row_count();

    for (std::size_t y = 0; y < count; y++) {

      auto row = TileRowView::make(view);

      for (std::size_t x = 0; x  < room->get_width(); x++) {

        auto* tile = room->access_tile(x, y + y_initial);

        auto tile_view = make_tile_view(row.get());

        if (tile) {
          tile_view->load_texture_data(textures->find_texture_data(tile->get_texture()));
        }

        row->add_tile(std::move(tile_view));
      }

      view->add_row(std::move(row));
    }
  }
  /// Updates the width of the room being viewed.
  /// @param width The width to assign the room.
  void update_room_width(std::size_t width) {

    auto* opened_room = opened_room_manager->get_current();
    if (!opened_room) {
      return;
    }

    auto* room = opened_room->get_room();

    auto original_width = room->get_width();

    room->set_width(width);

    if (original_width > width) {
      remove_columns(opened_room->get_view(), original_width - width);
    } else {
      create_columns(opened_room->get_view(), room, width - original_width);
    }
  }
  /// Removes columns from the room view.
  /// @param room_view The room view to remove columns from.
  /// @param count The number of columns to remove from each row in the room view.
  void remove_columns(RoomView* room_view, std::size_t count) {
    for (std::size_t y = 0; y < room_view->row_count(); y++) {
      room_view->get_row(y)->shrink(count);
    }
  }
  /// Creates columns in the room view.
  /// @param room_view The room view to put the columns into.
  /// @param room The room data to get the tile information from.
  /// @param count The number of columns to add to each row in the room view.
  void create_columns(RoomView* room_view, const Room* room, std::size_t count) {

    const auto* textures = project->access_texture_table();

    for (std::size_t y = 0; y < room_view->row_count(); y++) {

      auto* row = room_view->get_row(y);

      for (std::size_t x = 0; x < count; x++) {

        auto* tile = room->access_tile(x, y);

        auto tile_view = make_tile_view(row);

        if (tile) {
          tile_view->load_texture_data(textures->find_texture_data(tile->get_texture()));
        }

        row->add_tile(std::move(tile_view));
      }
    }
  }
};

} // namespace

ScopedPtr<QWidget> make_room_editor(Project* project, QWidget* parent) {
  return new RoomEditor(project, parent);
}

} // namespace tk

} // namespace herald
