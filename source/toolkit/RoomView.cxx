#include "RoomView.h"

#include "RoomModel.h"
#include "RoomToolMediator.h"
#include "TileModel.h"

#include <herald/ScopedPtr.h>

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QWidget>

namespace herald {

namespace tk {

namespace {

class TileView;

/// Used for modifying a tile based off of a room tool.
class TileViewModifier final : public RoomToolVisitor {
  /// The tile model to assign the texture to.
  TileModel& tile_model;
  /// The tile being modified.
  TileView& tile_view;
public:
  /// Constructs an instance of the modifier.
  /// @param t A reference to the tile view being modified.
  TileViewModifier(TileModel& m, TileView& t)
    : tile_model(m), tile_view(t) {}
protected:
  /// Applies the stamp to the tile view.
  /// @param stamp_tool The stamp tool containing the texture data.
  void apply_stamp(const StampTool& stamp_tool);
  /// Visits a stamp tool.
  /// This function will stamp the tile with the
  /// current texture assigned to the stamp tool.
  void visit(const StampTool& stamp_tool) override {
    apply_stamp(stamp_tool);
  }
};

/// The image displayed by the tile.
class TileImage final : public QWidget {
  /// The original pixmap to display.
  QPixmap pixmap;
public:
  /// Constructs a new tile image instance.
  /// @param parent A pointer to the parent widget.
  TileImage(QWidget* parent) : QWidget(parent) { }
  /// Loads the image data for the tile.
  /// @param data The image data to assign.
  /// @returns True on success, false on failure.
  bool load_data(const QByteArray& data) {
    auto success = pixmap.loadFromData(data);
    update();
    return success;
  }
protected:
  /// Overrides the paint event to draw the texture image
  /// onto the tile.
  void paintEvent(QPaintEvent* event) override {

    QPainter painter(this);

    if (!pixmap.isNull()) {
      painter.drawPixmap(0, 0, scale_pixmap(pixmap, size()));
    }

    QWidget::paintEvent(event);
  }
  /// Scales a tile pixmap to the appropriate size.
  /// @param in The pixmap to scale.
  /// @param out_size The size to scale the pixmap to.
  /// @returns The scaled pixmap.
  QPixmap scale_pixmap(const QPixmap& in, const QSize& out_size) {
    return in.scaled(out_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
};

/// A view of a single tile.
class TileView final : public QWidget {
  /// The layout of the tile view.
  QStackedLayout layout;
  /// The texture being displayed.
  TileImage image;
  /// The highlighter widget.
  QWidget highlight;
  /// A reference to the tile data.
  TileModel model;
  /// A pointer to the tool mediator,
  /// for when this tile view is clicked.
  RoomToolMediator* tool_mediator;
public:
  /// Constructs a new instance of a tile view.
  /// @param m A reference to a model for this tile.
  /// @param tm A pointer to the tool mediator for this tile view.
  /// @param parent A pointer to the parent widget.
  /// This is expected to be a @ref TileRowView widget.
  TileView(const TileModel& m, RoomToolMediator* tm, QWidget* parent)
    : QWidget(parent),
      layout(this),
      image(this),
      highlight(this),
      model(m),
      tool_mediator(tm) {

    layout.setStackingMode(QStackedLayout::StackAll);
    layout.addWidget(&highlight);
    layout.addWidget(&image);
  }
  /// Sets the pixmap data.
  /// @param data The pixmap data to assign the tile.
  /// @returns True on success, false on failure.
  bool load_pixmap_data(const QByteArray& data) {
    return image.load_data(data);
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
  /// Overrides the mouse press event in
  /// order to perform a tool action.
  void mousePressEvent(QMouseEvent* event) override {

    TileViewModifier modifier(model, *this);

    tool_mediator->get_current_tool()->accept(modifier);

    QWidget::mousePressEvent(event);
  }
};

void TileViewModifier::apply_stamp(const StampTool& stamp_tool) {
  tile_model.set_texture(stamp_tool.get_texture_name());
  tile_view.load_pixmap_data(stamp_tool.get_texture_data());
}

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
  /// Adds a tile to the row.
  /// @param tile The tile to add to the row.
  void add_tile(ScopedPtr<TileView>&& tile) {
    layout.addWidget(tile.get());
    tiles.emplace_back(std::move(tile));
  }
  /// Shrinks the row to a certain size.
  /// @param count The number of columns to shrink to.
  void shrink(std::size_t count) {
    for (auto i = tiles.size(); i > count; i--) {
      tiles.pop_back();
    }
  }
  /// Indicates the number of tiles in the row.
  std::size_t tile_count() const noexcept {
    return tiles.size();
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
  /// The type used for non-const iterators.
  using Iterator = std::vector<ScopedPtr<TileRowView>>::iterator;
  /// Constructs an instance of the tile map view.
  /// @param parent A pointer to the parent widget.
  TileMapView(QWidget* parent) : QWidget(parent), layout(this) {
    layout.setSpacing(0);
    layout.setMargin(0);
  }
  /// Adds a row to the tile map.
  /// @param row The row to add.
  void add_row(ScopedPtr<TileRowView>&& row) {
    layout.addWidget(row.get());
    rows.emplace_back(std::move(row));
  }
  /// Gets a row at a certain index.
  /// @param y The index of the row to get.
  /// @returns A pointer to the specified row.
  TileRowView* get_row(std::size_t y) noexcept {
    return (y < rows.size()) ? rows[y].get() : nullptr;
  }
  /// Indicates the number of rows in the tile map view.
  /// @returns The number of rows in the tile map view.
  std::size_t row_count() const noexcept {
    return rows.size();
  }
  /// Shrinks the name to a certain number of rows.
  /// @param count The number of rows to shrink to.
  void shrink(std::size_t count) {
    for (std::size_t i = rows.size(); i > count; i--) {
      rows.pop_back();
    }
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
  /// A model of the room tools.
  RoomToolMediator* tool_mediator;
public:
  /// Constructs a new instance of the room view.
  /// @param parent A pointer to the parent widget.
  /// @param m The room model to be viewed.
  /// @param tm The room tool model, used for editing.
  RoomViewImpl(RoomModel* m, RoomToolMediator* tm, QWidget* parent) : model(m), tool_mediator(tm) {

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

    grid_view->set_height(height);

    if (height < tile_map_view->row_count()) {
      tile_map_view->shrink(height);
    } else {
      add_rows(height - tile_map_view->row_count());
    }
  }
  /// Adds rows to the tile map view.
  void add_rows(std::size_t count) {

    auto y = tile_map_view->row_count();

    for (std::size_t i = 0; i < count; i++) {
      tile_map_view->add_row(make_row(y + i));
    }
  }
  /// Creates a new row for the tile map view.
  /// @param y The index of the row to be made.
  ScopedPtr<TileRowView> make_row(std::size_t y) {

    auto w = model->get_width();

    auto row = ScopedPtr<TileRowView>::make(tile_map_view.get());

    auto x = row->tile_count();

    for (std::size_t i = 0; i < w; i++) {
      row->add_tile(make_tile(x + i, y, row.get()));
    }

    return row;
  }
  /// Creates a new tile instance.
  /// @param x The X coordinate of the tile to be made.
  /// @param y The Y coordinate of the tile to be made.
  /// @param parent A pointer to the parent widget.
  /// @returns A new tile view instance.
  ScopedPtr<TileView> make_tile(std::size_t x, std::size_t y, QWidget* parent) {
    return new TileView(TileModel(model, x, y), tool_mediator, parent);
  }
  /// Adjusts the tile width of the
  /// view to the width of the model.
  void adjust_width_to_model() {

    auto width = model->get_width();

    grid_view->set_width(width);

    for (std::size_t y = 0; y < tile_map_view->row_count(); y++) {
      auto* row = tile_map_view->get_row(y);
      if (row->tile_count() > width) {
        row->shrink(width);
      } else {
        add_tiles(row, y, width - row->tile_count());
      }
    }
  }
  /// Adds tiles to a row.
  /// @param row The row to add tiles to.
  /// @param y The index of the row.
  /// @param count The number of tiles to add to the row.
  void add_tiles(TileRowView* row, std::size_t y, std::size_t count) {

    auto x = row->tile_count();

    for (std::size_t i = 0; i < count; i++) {
      row->add_tile(make_tile(x + i, y, row));
    }
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

ScopedPtr<RoomView> RoomView::make(RoomModel* model, RoomToolMediator* tool_mediator, QWidget* parent) {
  return new RoomViewImpl(model, tool_mediator, parent);
}

} // namespace tk

} // namespace herald
