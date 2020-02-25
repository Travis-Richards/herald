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

  }
};

/// A view of a row within a tile map.
class TileRowView final : public QWidget {
  /// The layout of the row.
  QHBoxLayout layout;
  /// The columns within the row.
  std::vector<ScopedPtr<TileView>> columns;
public:
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

  }
  /// Sets the height of the tile map.
  /// @param height The height to set
  /// the tile map to, in terms of tiles.
  void set_height(std::size_t height) {
    rows.resize(height);
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

    QObject::connect(m, &RoomModel::room_changed, room_change_functor);

    if (m->is_valid()) {
      // Map data
    }
  }
  /// Accesses a pointer to the widget containing the room view.
  /// @returns A pointer to the room view widget.
  QWidget* get_widget() noexcept override {
    return root_widget.get();
  }
protected:
  /// Handles the case of a room being changed.
  /// A room change requires that all the current
  /// room data be discarded and the new room data
  /// to be mapped to the view.
  void on_room_changed() {
    qDebug() << "ROOM CHANGED";
  }
};

} // namespace

ScopedPtr<RoomView> RoomView::make(RoomModel* model, QWidget* parent) {
  return new RoomViewImpl(model, parent);
}

} // namespace tk

} // namespace herald
