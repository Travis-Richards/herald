#include "RoomView.h"

#include "RoomModel.h"

#include <herald/ScopedPtr.h>

#include <QGridLayout>
#include <QStackedLayout>
#include <QWidget>

#include <QDebug>

namespace herald {

namespace tk {

namespace {

/// A view of a single tile.
class TileView final : public QWidget {
public:
  /// Constructs a new instance of a tile view.
  /// @param parent A pointer to the parent widget.
  /// This is expected to be a @ref TileRowView widget.
  TileView(QWidget* parent) : QWidget(parent) {
    setStyleSheet("border: 1px solid blue");
  }
};

/// A view of a row within a tile map.
class TileRowView final : public QWidget {
  /// The layout of the row.
  QHBoxLayout layout;
  /// The columns within the row.
  std::vector<ScopedPtr<TileView>> columns;
public:
  /// Constructs a new instance of a row view.
  /// @param parent A pointer to the parent widget.
  TileRowView(QWidget* parent) : QWidget(parent), layout(this) {
    layout.setSpacing(0);
  }
  /// Sets the width of the row.
  /// @param width The number of tiles
  /// to put into this row.
  void set_width(std::size_t width) {
    if (width > columns.size()) {
      expand_columns(width - columns.size());
    } else if (width < columns.size()) {
      shrink_columns(columns.size() - width);
    }
  }
protected:
  /// Adds columns to the row.
  /// @param count The number of columns to add.
  void expand_columns(std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {

      auto tile = ScopedPtr<TileView>::make(this);

      layout.addWidget(tile.get());

      columns.emplace_back(std::move(tile));
    }
  }
  /// Removes columns from the row.
  /// @param count The number of columns to remove.
  void shrink_columns(std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {
      columns.pop_back();
    }
  }
};

/// A view of the room's tiles.
class TileMapView final : public QWidget {
  /// The rows of the tile map.
  std::vector<ScopedPtr<TileRowView>> rows;
  /// The layout of the tile map view.
  /// This particular view is the
  /// vertical layout of the tile rows.
  QVBoxLayout layout;
public:
  /// Constructs an instance of the tile map view.
  /// @param parent A pointer to the parent widget.
  TileMapView(QWidget* parent) : QWidget(parent), layout(this) {
    layout.setSpacing(0);
    layout.setMargin(0);
  }
  /// Sets the height of the tile map.
  /// @param height The height to set
  /// the tile map to, in terms of tiles.
  void set_height(std::size_t height) {
    if (height > rows.size()) {
      expand_rows(height - rows.size());
    } else if (height < rows.size()) {
      shrink_rows(rows.size() - height);
    }
  }
  /// Sets the width of the tile map.
  void set_width(std::size_t width) {
    for (auto& row : rows) {
      row->set_width(width);
    }
  }
protected:
  /// Adds rows to the tile map.
  void expand_rows(std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {

      auto row = ScopedPtr<TileRowView>::make(this);

      layout.addWidget(row.get());

      rows.emplace_back(std::move(row));
    }
  }
  /// Removes rows from the tile map.
  void shrink_rows(std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {
      rows.pop_back();
    }
  }
};

/// The implementation of the room view interface.
class RoomViewImpl final : public RoomView {
  /// A pointer to the widget containing the room view.
  ScopedPtr<QWidget> root_widget;
  /// The layout of the root widget.
  /// A stacked layout allows for multiple
  /// widgets to be stacked on top of each other.
  /// This closely emulates what the game's appearance is.
  ScopedPtr<QStackedLayout> root_layout;
  /// A view of the tile map.
  ScopedPtr<TileMapView> tile_map_view;
  /// A pointer to the room model being modified.
  RoomModel* model;
public:
  /// Constructs a new instance of the room view.
  /// @param parent A pointer to the parent widget.
  /// @param m The room model to be viewed.
  RoomViewImpl(RoomModel* m, QWidget* parent) : model(m) {

    root_widget = ScopedPtr<QWidget>::make(parent);

    tile_map_view = ScopedPtr<TileMapView>::make(root_widget.get());

    auto room_change_functor = [this]() { on_room_changed(); };

    QObject::connect(model, &RoomModel::room_changed, room_change_functor);

    if (model->is_valid()) {
      // Map data
    }

    root_layout = ScopedPtr<QStackedLayout>::make(root_widget.get());

    root_layout->addWidget(tile_map_view.get());
  }
  /// Accesses a pointer to the widget containing the room view.
  /// @returns A pointer to the room view widget.
  QWidget* get_widget() noexcept override {
    return root_widget.get();
  }
protected:
  /// Adjusts the tile height of the
  /// view to the height of the model.
  void adjust_height_to_model() {
    tile_map_view->set_height(model->get_height());
  }
  /// Adjusts the tile width of the
  /// view to the width of the model.
  void adjust_width_to_model() {
    tile_map_view->set_width(model->get_width());
  }
  /// Adjusts the width and height of
  /// the view to the current data in
  /// the room model.
  void adjust_size_to_model() {
    adjust_height_to_model();
    adjust_width_to_model();
  }
  /// Handles the case of a room being changed.
  /// A room change requires that all the current
  /// room data be discarded and the new room data
  /// to be mapped to the view.
  void on_room_changed() {
    adjust_size_to_model();
  }
};

} // namespace

ScopedPtr<RoomView> RoomView::make(RoomModel* model, QWidget* parent) {
  return new RoomViewImpl(model, parent);
}

} // namespace tk

} // namespace herald
