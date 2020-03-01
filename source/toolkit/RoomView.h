#pragma once

#include <QWidget>

#include <cstddef>

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// This class represents a single
/// view of a tile.
class TileView : public QWidget {
  Q_OBJECT
public:
  /// Constructs a new tile view instance.
  /// @param parent A pointer to the parent widget.
  /// @returns A new instance of a tile view.
  static ScopedPtr<TileView> make(QWidget* parent);
  /// Just a stub.
  virtual ~TileView() {}
  /// Loads the texture data to be used by the tile view.
  /// @param data The texture data to load for the tile view.
  /// @returns True if the texture data is valid, false otherwise.
  virtual bool load_texture_data(const QByteArray& data) = 0;
signals:
  /// This signal gets emitted when the tile view is clicked.
  /// @param tile_view A pointer to the tile view instance.
  void clicked(TileView* tile_view);
protected:
  /// Constructs a base tile view.
  /// @param parent A pointer to the parent widget.
  TileView(QWidget* parent) : QWidget(parent) {}
};

/// This is a class that represents
/// a row of tiles in the room view.
class TileRowView : public QWidget {
public:
  /// Creates a new tile row view.
  /// @param parent A pointer to the parent widget.
  static ScopedPtr<TileRowView> make(QWidget* parent);
  /// Just a stub.
  virtual ~TileRowView() {}
  /// Adds a tile view to the row.
  /// @param tile The tile view to add to the row.
  virtual void add_tile(ScopedPtr<TileView>&& tile) = 0;
  /// Accesses a tile at a specific location.
  /// @param x The coordinate of the tile within the row.
  /// This value must start at zero.
  /// @returns On success, a pointer to the tile.
  /// On failure, a null pointer.
  virtual const TileView* get_tile(std::size_t x) const noexcept = 0;
  /// Accesses a tile at a specific location.
  /// @param x The coordinate of the tile within the row.
  /// This value must start at zero.
  /// @returns On success, a pointer to the tile.
  /// On failure, a null pointer.
  virtual TileView* get_tile(std::size_t x) noexcept = 0;
  /// Removes a certain number of columns from the row view.
  /// @param count The number of columns to remove.
  virtual void shrink(std::size_t count) = 0;
  /// Indicates the number of tiles in the row.
  virtual std::size_t tile_count() const noexcept = 0;
protected:
  /// Constructs the base row view.
  /// @param parent A pointer to the parent widget.
  TileRowView(QWidget* parent) : QWidget(parent) {}
};

/// Used to display a room within the room editor.
class RoomView : public QWidget {
public:
  /// Creates a new room view instance.
  /// @param parent A pointer to the parent widget.
  /// @returns A new room view instance.
  static ScopedPtr<RoomView> make(QWidget* parent);
  /// Just a stub.
  virtual ~RoomView() {}
  /// Adds a row to the room view.
  /// @param row The row to add to the room view.
  virtual void add_row(ScopedPtr<TileRowView>&& row) = 0;
  /// Gets a row from the room view.
  /// @param y The index of the row to get.
  /// @returns A pointer to the specified row.
  /// If the index is out of bounds, a null pointer is returned.
  virtual TileRowView* get_row(std::size_t y) = 0;
  /// Finds the location of a tile.
  /// @param tile_view The tile view to find.
  /// @param x The variable to receive the X coordinate.
  /// @param y The variable to receive the Y coordinate.
  /// @returns True on success, false on failure.
  virtual bool find_location(TileView* tile_view, std::size_t& x, std::size_t& y) const = 0;
  /// Hides the grid of the room.
  virtual void hide_grid() = 0;
  /// Shows the grid of the room.
  virtual void show_grid() = 0;
  /// Removes the last row in the view.
  virtual void remove_last_row() = 0;
  /// Indicates the number of rows currently in the room view.
  /// @returns The number of rows in the room view.
  virtual std::size_t row_count() const noexcept = 0;
protected:
  /// Constructs a new room view instance.
  /// @param parent A pointer to the parent widget.
  RoomView(QWidget* parent) : QWidget(parent) {}
};

} // namespace tk

} // namespace herald
