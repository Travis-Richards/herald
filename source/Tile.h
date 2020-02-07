#pragma once

#include <QObject>

/// Represents one tile within a room.
class Tile : public QObject {
  Q_OBJECT
public:
  /// Constructs the tile instance.
  /// @param parent A pointer to the parent object.
  Tile(QObject* parent = nullptr);
  /// Indicates whether or not the tile has a particular texture.
  bool has_texture(int texture) const noexcept;
  /// Accesses flags associated with the tile.
  int get_flags() const noexcept;
  /// Gets the index of the frame that
  /// this tile is currently at.
  int get_frame() const noexcept;
  /// Gets the index of the texture animation
  /// assigned to this tile.
  int get_texture() const noexcept;
  /// Assigns flags to the tile.
  void set_flags(int flags) noexcept;
  /// Assigns the frame index.
  void set_frame(int index) noexcept;
  /// Assigns the frame offset.
  /// @param offset The frame offset to assign.
  void set_frame_offset(int offset) noexcept;
  /// Assigns the texture animation index.
  void set_texture(int index) noexcept;
private:
  /// The flags assigned to the tile.
  int flags;
  /// The frame that the texture animation
  /// for the tile is currently at.
  int frame;
  /// The frame offset is a value added to
  /// the current frame to offset the tile
  /// animation time and make it appear to
  /// be different from other tiles with
  /// the same animation.
  int frame_offset;
  /// The index of the texture animation
  /// assigned to this tile.
  int texture;
};
