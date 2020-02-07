#pragma once

#include <QObject>

class Tile;

/// Represents a room.
/// A room is made up of a matrix of tiles.
class Room : public QObject {
public:
  /// Creates a null room, as a placeholder.
  /// @param parent A pointer to the parent object.
  static Room* make_null(QObject* parent);
  /// Constructs the base room instance.
  Room(QObject* parent);
  /// Just a stub.
  virtual ~Room() {}
  /// Indicates whether or not the room is empty.
  /// @returns True if the room is empty, false otherwise.
  inline bool empty() const noexcept {
    return !width() || !height();
  }
  /// Accesses a tile at a specified location.
  /// @param x The X coordinate of the tile to get.
  /// @param y The Y coordinate of the tile to get.
  /// @returns A pointer to the tile at the specified location.
  virtual const Tile* get_tile(int x, int y) const noexcept = 0;
  /// Reassigns a frame index to all tiles containing a particular texture.
  /// @param texture The index of the texture being updated.
  /// @param frame The new frame index of the texture.
  virtual void update_tile_frames(int texture, int frame) noexcept = 0;
  /// Accesses the width of the room.
  virtual int width() const noexcept = 0;
  /// Accesses the height of the room.
  virtual int height() const noexcept  = 0;
};
