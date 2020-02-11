#pragma once

#include <QObject>

class Matrix;
class Tile;

/// Represents a room.
/// A room is made up of a matrix of tiles.
class Room : public QObject {
  Q_OBJECT
public:
  /// Creates a new room instance.
  /// @param parent The parent object to assign the room.
  static Room* make(QObject* parent);
  /// Constructs the base room instance.
  Room(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~Room() {}
  /// Clears the modification flags for the tiles.
  virtual void clear_mod_flags() = 0;
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
  /// Accesses the height of the room.
  virtual int height() const noexcept  = 0;
  /// Maps the animation matrix onto the room.
  /// @param matrix The animation matrix to map onto the room.
  virtual void map_animation_matrix(const Matrix& matrix) = 0;
  /// Resizes the room.
  /// @param width The width to assign the room.
  /// @param height The height to assign the room.
  virtual void resize(int width, int height) = 0;
  /// Sets the modification flag for each tile.
  virtual void set_mod_flags() noexcept = 0;
  /// Reassigns a frame index to all tiles containing a particular texture.
  /// @param texture The index of the texture being updated.
  /// @param frame The new frame index of the texture.
  virtual void update_tile_frames(int texture, int frame) noexcept = 0;
  /// Accesses the width of the room.
  virtual int width() const noexcept = 0;
};
