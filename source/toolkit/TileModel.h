#pragma once

#include <QObject>

#include <cstddef>

class QString;

namespace herald {

namespace tk {

class RoomModel;

/// This is the Qt style data model
/// of a single room tile.
class TileModel : public QObject {
  Q_OBJECT
public:
  /// Constructs a new instance of the tile model.
  /// @param m A pointer to the room model.
  /// @param x The X coordinate of this tile.
  /// @parma y The Y coordinate of this tile.
  TileModel(RoomModel* m,
            std::size_t x,
            std::size_t y);
  /// Copies the tile model.
  /// Since the tile is accessed by reference, this doesn't copy any data.
  /// @param other The tile model to copy.
  TileModel(const TileModel& other);
  /// Accesses the name of the texture being displayed.
  /// @returns The name of the texture that the tile is displaying.
  /// An empty string indicates that there is no texture.
  QString get_texture() const;
  /// Indicates whether or not this
  /// model actually points to a tile.
  /// It may not if the tile has not
  /// been created yet.
  /// @returns True if the tile model
  /// is valid, false if it's not.
  bool is_valid() const;
  /// Assigns the texture to be displayed by this tile.
  /// @param name The name of the texture to display.
  void set_texture(const QString& name);
signals:
  /// This signal is emitted whenever the tile changes.
  void changed();
private:
  /// The room model that this tile corresponds to.
  RoomModel* model;
  /// The X coordinate of this tile.
  std::size_t x;
  /// The Y coordinate of this tile.
  std::size_t y;
};

} // namespace tk

} // namespace herald
