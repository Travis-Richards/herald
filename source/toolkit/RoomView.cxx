#include "RoomView.h"

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
class TileViewImpl final : public TileView {
  /// The layout of the tile view.
  QStackedLayout layout;
  /// The texture being displayed.
  TileImage image;
  /// The highlighter widget.
  QWidget highlight;
public:
  /// Constructs a new instance of a tile view.
  /// @param parent A pointer to the parent widget.
  /// This is expected to be a @ref TileRowView widget.
  TileViewImpl(QWidget* parent)
    : TileView(parent),
      layout(this),
      image(this),
      highlight(this) {

    layout.setStackingMode(QStackedLayout::StackAll);
    layout.addWidget(&highlight);
    layout.addWidget(&image);
  }
  /// Sets the pixmap data.
  /// @param data The pixmap data to assign the tile.
  /// @returns True on success, false on failure.
  bool load_texture_data(const QByteArray& data) override {
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

    emit clicked(this);

    QWidget::mousePressEvent(event);
  }
};

/// A view of a row within a tile map.
class TileRowViewImpl final : public TileRowView {
  /// The layout of the row.
  QHBoxLayout layout;
  /// The columns within the row.
  std::vector<ScopedPtr<TileView>> tiles;
public:
  /// Constructs a new instance of a row view.
  /// @param parent A pointer to the parent widget.
  TileRowViewImpl(QWidget* parent) : TileRowView(parent), layout(this) {
    layout.setSpacing(0);
    layout.setMargin(0);
  }
  /// Adds a tile to the row.
  /// @param tile The tile to add to the row.
  void add_tile(ScopedPtr<TileView>&& tile) override {
    layout.addWidget(tile.get());
    tiles.emplace_back(std::move(tile));
  }
  /// Gets a tile at a specified location.
  /// @param x The index within the row to get the tile from.
  /// @returns On success, a pointer to the tile.
  /// On failure, a null pointer.
  const TileView* get_tile(std::size_t x) const noexcept override {
    if (x >= tiles.size()) {
      return nullptr;
    } else {
      return tiles[x].get();
    }
  }
  /// Shrinks the row to a certain size.
  /// @param count The number of columns to shrink to.
  void shrink(std::size_t count) {
    for (auto i = tiles.size(); i > count; i--) {
      tiles.pop_back();
    }
  }
  /// Indicates the number of tiles in the row.
  std::size_t tile_count() const noexcept override {
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
  /// Deletes all rows in the tile map.
  /// Gets a row at a certain index.
  /// @param y The index of the row to get.
  /// @returns A pointer to the specified row.
  TileRowView* get_row(std::size_t y) noexcept {
    return (y < rows.size()) ? rows[y].get() : nullptr;
  }
  /// Gets that maximum number of tiles that appear in one row.
  std::size_t get_max_width() const {

    std::size_t max = 0;

    for (std::size_t i = 0; i < row_count(); i++) {
      auto tmp = rows[i]->tile_count();
      if (tmp > max) {
        max = tmp;
      }
    }

    return max;
  }
  /// Gets a row at a certain index.
  /// @param y The index of the row to get.
  /// @returns A pointer to the specified row.
  const TileRowView* get_row(std::size_t y) const noexcept {
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

/// Used to paint a grid view on top of the room view.
class GridPainter final {
  /// The grid painter.
  QPainter painter;
  /// The number of vertical lines.
  int width;
  /// The number of horizontal lines.
  int height;
public:
  /// Constructs a new instance of the grid painter.
  /// @param device The device to paint on.
  /// @param w The width of the grid.
  /// @param h The height of the grid.
  GridPainter(QPaintDevice* device, int w, int h) : painter(device), width(w), height(h) { }
  /// Paints the grid onto the paint device.
  void paint() {

    QPen pen(Qt::DashLine);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);

    for (auto y = 0; y < height; y++) {

      draw_horizontal_line(painter, y);

      for (auto x = 0; x < width; x++) {
        draw_vertical_line(painter, x, y);
      }

      draw_vertical_line(painter, width, y);
    }

    draw_horizontal_line(painter, height);
  }
  /// Draws a horizontal grid line.
  /// @param painter The painter to draw the horizontal grid line with.
  /// @param y The vertical grid offset to draw the line at.
  void draw_horizontal_line(QPainter& painter, int y) {
    auto p1 = to_point(painter, 0, y);
    auto p2 = to_point(painter, width, y);
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

    auto x_max = width;
    auto y_max = height;

    auto pixel_x = (painter.device()->width() * x) / x_max;
    auto pixel_y = (painter.device()->height() * y) / y_max;

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
  /// The layout of the root widget.
  /// A stacked layout allows for multiple
  /// widgets to be stacked on top of each other.
  /// This closely emulates what the game's appearance is.
  ScopedPtr<QStackedLayout> root_layout;
  /// A view of the tile map.
  ScopedPtr<TileMapView> tile_map_view;
  /// Whether or not the grid is enabled.
  bool grid_enabled;
public:
  /// Constructs a new instance of the room view.
  /// @param parent A pointer to the parent widget.
  RoomViewImpl(QWidget* parent) : RoomView(parent), grid_enabled(true) {

    tile_map_view = ScopedPtr<TileMapView>::make(this);

    root_layout = ScopedPtr<QStackedLayout>::make(this);
    root_layout->setStackingMode(QStackedLayout::StackAll);
    root_layout->addWidget(tile_map_view.get());
  }
  /// Adds a row to the room view.
  /// @param row The row to add to the room view.
  void add_row(ScopedPtr<TileRowView>&& row) override {
    tile_map_view->add_row(std::move(row));
  }
  /// Locates the position of a tile view.
  /// @param tile_view The tile view to search for.
  /// @returns True if it's found, false otherwise.
  bool find_location(TileView* tile_view, std::size_t& x_out, std::size_t& y_out) const override {

    for (std::size_t y = 0; y < tile_map_view->row_count(); y++) {

      auto* row = tile_map_view->get_row(y);

      for (std::size_t x = 0; x < row->tile_count(); x++) {

        const auto* tile = row->get_tile(x);
        if (tile == tile_view) {
          x_out = x;
          y_out = y;
          return true;
        }
      }
    }

    return false;
  }
  /// Hides the grid view.
  void hide_grid() override {
    if (grid_enabled) {
      grid_enabled = false;
      update();
    }
  }
  /// Shows the grid view.
  void show_grid() override {
    if (!grid_enabled) {
      grid_enabled = true;
      update();
    }
  }
protected:
  /// Overrides the paint event in order to draw the grid.
  /// @param event A pointer to the event info.
  void paintEvent(QPaintEvent* event) override {

    if (grid_enabled) {
      paint_grid(event);
    }

    RoomView::paintEvent(event);
  }
  /// Paints a grid onto the widget.
  void paint_grid(QPaintEvent*) {

    auto w = (int) tile_map_view->get_max_width();
    auto h = (int) tile_map_view->row_count();

    GridPainter painter(this, w, h);

    painter.paint();
  }
};

} // namespace

ScopedPtr<TileView> TileView::make(QWidget* parent) {
  return new TileViewImpl(parent);
}

ScopedPtr<TileRowView> TileRowView::make(QWidget* parent) {
  return new TileRowViewImpl(parent);
}

ScopedPtr<RoomView> RoomView::make(QWidget* parent) {
  return new RoomViewImpl(parent);
}

} // namespace tk

} // namespace herald
