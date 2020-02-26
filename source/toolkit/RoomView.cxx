#include "RoomView.h"

#include "RoomModel.h"

#include <herald/ScopedPtr.h>

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QStackedLayout>
#include <QWidget>

#include <QDebug>

namespace herald {

namespace tk {

namespace {

/// A view of a single tile.
class TileView final : public QWidget {
  /// The layout of the tile view.
  QStackedLayout layout;
  /// The tile texture being displayed.
  QLabel texture;
  /// The highlighter widget.
  QWidget highlight;
public:
  /// Constructs a new instance of a tile view.
  /// @param parent A pointer to the parent widget.
  /// This is expected to be a @ref TileRowView widget.
  TileView(QWidget* parent)
    : QWidget(parent),
      layout(this),
      texture(this),
      highlight(this) {

    layout.setStackingMode(QStackedLayout::StackAll);
    layout.addWidget(&texture);
    layout.addWidget(&highlight);
  }
protected:
  /// Overrides the mouse over event to highlight the tile.
  void enterEvent(QEvent* event) override {
    highlight.setStyleSheet("background-color: rgba(0, 0, 255, 32)");
    QWidget::enterEvent(event);
  }
  /// Overrides the mouse leave event to
  /// remove the highlight from the tile.
  void leaveEvent(QEvent* event) override {
    highlight.setStyleSheet("");
    QWidget::leaveEvent(event);
  }
};

/// A view of a row within a tile map.
class TileRowView final : public QWidget {
  /// The layout of the row.
  QHBoxLayout layout;
  /// The columns within the row.
  std::vector<ScopedPtr<TileView>> tiles;
public:
  /// Constructs a new instance of a row view.
  /// @param parent A pointer to the parent widget.
  TileRowView(QWidget* parent) : QWidget(parent), layout(this) {
    layout.setSpacing(0);
    layout.setMargin(0);
  }
  /// Sets the width of the row.
  /// @param width The number of tiles
  /// to put into this row.
  void set_width(std::size_t width) {
    if (width > tiles.size()) {
      expand_columns(width - tiles.size());
    } else if (width < tiles.size()) {
      shrink_columns(tiles.size() - width);
    }
  }
protected:
  /// Adds columns to the row.
  /// @param count The number of columns to add.
  void expand_columns(std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {
      add_tile();
    }
  }
  /// Removes columns from the row.
  /// @param count The number of columns to remove.
  void shrink_columns(std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {
      tiles.pop_back();
    }
  }
  /// Adds a tile to the row.
  void add_tile() {
    auto tile = ScopedPtr<TileView>::make(this);
    layout.addWidget(tile.get());
    tiles.emplace_back(std::move(tile));
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
  /// @param width The width to view the new rows.
  /// @param height The height to set
  /// the tile map to, in terms of tiles.
  void set_height(std::size_t width, std::size_t height) {
    if (height > rows.size()) {
      expand_rows(height - rows.size(), width);
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
  void expand_rows(std::size_t count, std::size_t width) {
    for (std::size_t i = 0; i < count; i++) {
      add_tile_row(width);
    }
  }
  /// Removes rows from the tile map.
  void shrink_rows(std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {
      rows.pop_back();
    }
  }
  /// Adds a tile row to the room view.
  /// @param width The width to give the new rows.
  void add_tile_row(std::size_t width) {

    auto row = ScopedPtr<TileRowView>::make(this);

    row->set_width(width);

    layout.addWidget(row.get());

    rows.emplace_back(std::move(row));
  }
};

/// A view of the tile map grid.
class GridView final : public QWidget {
  /// The number of vertical lines.
  int grid_width;
  /// The number of horizontal lines.
  int grid_height;
public:
  /// Constructs a grid view.
  /// @param parent A pointer to the parent widget.
  GridView(QWidget* parent) : QWidget(parent), grid_width(1), grid_height(1) {

  }
  /// Sets the grid width.
  /// @param w The number of vertical lines to draw.
  void set_width(int w) {
    grid_width = (w < 1) ? 1 : w;
    update();
  }
  /// Sets the grid height.
  /// @param w The number of horizontal lines to draw.
  void set_height(std::size_t h) {
    grid_height = (h < 1) ? 1 : h;
    update();
  }
protected:
  /// Overrides the paint event to draw the grid pattern.
  /// @param event A pointer to the paint event instance.
  void paintEvent(QPaintEvent* event) override {

    QPainter painter(this);

    QPen pen(Qt::DashLine);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);

    for (auto y = 0; y < grid_height; y++) {

      draw_horizontal_line(painter, y);

      for (auto x = 0; x < grid_width; x++) {
        draw_vertical_line(painter, x, y);
      }

      draw_vertical_line(painter, grid_width, y);
    }

    draw_horizontal_line(painter, grid_height);

    QWidget::paintEvent(event);
  }
  /// Draws a horizontal grid line.
  /// @param painter The painter to draw the horizontal grid line with.
  /// @param y The vertical grid offset to draw the line at.
  void draw_horizontal_line(QPainter& painter, int y) {
    auto p1 = to_point(painter, 0, y);
    auto p2 = to_point(painter, grid_width, y);
    painter.drawLine(p1, p2);
  }
  /// Draws a vertical line, starting at @p y and ending
  /// one grid unit downwards.
  void draw_vertical_line(QPainter& painter, int x, int y) {
    auto p1 = to_point(painter, x, y);
    auto p2 = to_point(painter, x, y + 1);
    painter.drawLine(p1, p2);
  }
  /// Converts a grid coordinate to a pixel location.
  /// @param painter The painter to use the line.
  /// The painter's pen gets used to adjust the grid
  /// point for the pixel boundaries.
  /// @param x The X grid coordinate.
  /// @param y The Y grid coordinate.
  /// @returns The pixel point for the grid coordinate.
  QPoint to_point(QPainter& painter, int x, int y) {

    auto x_max = grid_width;
    auto y_max = grid_height;

    auto pixel_x = (width() * x) / x_max;
    auto pixel_y = (height() * y) / y_max;

    auto pen_size = painter.pen().width();

    if (y == 0) {
      pixel_y += pen_size;
    } else if (y == y_max) {
      pixel_y -= pen_size;
    }

    if (x == 0) {
      pixel_x += pen_size;
    } else if (x == x_max) {
      pixel_x -= pen_size;
    }

    return QPoint(pixel_x, pixel_y);
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
  /// A view of the tile grid.
  ScopedPtr<GridView> grid_view;
  /// A pointer to the room model being modified.
  RoomModel* model;
public:
  /// Constructs a new instance of the room view.
  /// @param parent A pointer to the parent widget.
  /// @param m The room model to be viewed.
  RoomViewImpl(RoomModel* m, QWidget* parent) : model(m) {

    root_widget = ScopedPtr<QWidget>::make(parent);

    tile_map_view = ScopedPtr<TileMapView>::make(root_widget.get());

    grid_view = ScopedPtr<GridView>::make(root_widget.get());

    auto room_change_functor = [this]() { on_room_changed(); };

    auto width_change_functor = [this]() { adjust_width_to_model(); };

    auto height_change_functor = [this]() { adjust_height_to_model(); };

    QObject::connect(model, &RoomModel::room_changed, room_change_functor);
    QObject::connect(model, &RoomModel::width_changed, width_change_functor);
    QObject::connect(model, &RoomModel::height_changed, height_change_functor);

    if (model->is_valid()) {
      // Map data
    }

    root_layout = ScopedPtr<QStackedLayout>::make(root_widget.get());
    root_layout->setStackingMode(QStackedLayout::StackAll);
    root_layout->addWidget(tile_map_view.get());
    root_layout->addWidget(grid_view.get());
  }
  /// Accesses a pointer to the widget containing the room view.
  /// @returns A pointer to the room view widget.
  QWidget* get_widget() noexcept override {
    return root_widget.get();
  }
  /// Hides the grid view.
  void hide_grid() override {
    grid_view->hide();
  }
  /// Shows the grid view.
  void show_grid() override {
    grid_view->show();
  }
protected:
  /// Adjusts the tile height of the
  /// view to the height of the model.
  void adjust_height_to_model() {

    auto height = model->get_height();

    tile_map_view->set_height(model->get_width(), height);

    grid_view->set_height(height);
  }
  /// Adjusts the tile width of the
  /// view to the width of the model.
  void adjust_width_to_model() {
    tile_map_view->set_width(model->get_width());
    grid_view->set_width(model->get_width());
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
