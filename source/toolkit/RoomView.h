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
  /// Clears the contens of a room view.
  virtual void clear() = 0;
  /// Hides the grid of the room.
  virtual void hide_grid() = 0;
  /// Shows the grid of the room.
  virtual void show_grid() = 0;
protected:
  /// Constructs a new room view instance.
  /// @param parent A pointer to the parent widget.
  RoomView(QWidget* parent) : QWidget(parent) {}
};

} // namespace tk

} // namespace herald
