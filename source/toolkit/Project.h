#pragma once

#include <QObject>

#include <cstddef>

class QByteArray;
class QString;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// The texture table is the table
/// of images being used by the project.
/// Textures are indexed by name and
/// loaded by path.
class TextureTable : public QObject {
  Q_OBJECT
public:
  /// Just a stub.
  virtual ~TextureTable() { }
  /// Makes a new entry for a texture
  /// in the texture table.
  /// @param path The path of the texture to add to the table.
  /// @returns The ID of the texture.
  virtual QString add(const QString& path) = 0;
  /// Gets the image data of a texture.
  /// @param index The index of the texture to get the data for.
  /// @returns The image data of the specified image.
  /// If the texture isn't found, then an empty byte array is returned.
  virtual QByteArray get_data(std::size_t index) const = 0;
  /// Gets the name of a texture.
  /// @param index The index of the texture to get the name of.
  /// @returns The name of the specified texture.
  /// If the index is out of bounds, then an empty string is returned.
  virtual QString get_name(std::size_t index) const = 0;
  /// Removes a texture from the texture table.
  /// @param index The index of the item to remove.
  /// @returns True on success, false on failure.
  virtual bool remove(std::size_t index) = 0;
  /// Renames a texture.
  /// @param index The index of the texture to rename.
  /// @param name The new name to give the texture.
  /// @returns True on success, false on failure.
  virtual bool rename(std::size_t index, const QString& name) = 0;
  /// Indicates the size of the texture table.
  /// @returns The size of the texture table.
  virtual std::size_t size() const noexcept = 0;
protected:
  /// Constructs the base of the object table.
  /// @param parent A pointer to the parent object.
  inline TextureTable(QObject* parent) : QObject(parent) {}
signals:
  /// This signal is emitted when a item in the table is renamed.
  /// @param index The index of the item that was renamed.
  void renamed(std::size_t index);
};

/// This is the interface for a single
/// tile within a room.
class Tile : public QObject {
public:
  /// Just a stub.
  virtual ~Tile() {}
  /// Gets the name of the texture being displayed.
  /// If the tile does not currently have a texture,
  /// then an empty string is returned instead.
  virtual const QString& get_texture() const noexcept = 0;
  /// Sets the texture to be displayed by the tile.
  /// @param name The name of the texture to display.
  virtual void set_texture(const QString& name) = 0;
protected:
  /// Constructs a new instance of a tile base.
  /// @param parent A pointer to the parent object.
  Tile(QObject* parent) : QObject(parent) {}
};

/// This is the interface for a single room.
class Room : public QObject {
  /// The width of the room, in terms of tiles.
  std::size_t width;
  /// The height of the room, in terms of tiles.
  std::size_t height;
public:
  /// Just a stub.
  virtual ~Room() {}
  /// Accesses a certain tile from the room.
  /// @param x The X coordinate of the tile to access.
  /// @param y The Y coordinate of the tile to access.
  /// @returns A pointer to the specified tile.
  /// If the tile doesn't exist, then a null pointer is returned.
  virtual const Tile* access_tile(std::size_t x, std::size_t y) const noexcept = 0;
  /// Gets a tile for modification.
  /// If the tile doesn't exist, then it is made.
  /// @param x The X coordinate of the tile to get.
  /// @param y The Y coordinate of the tile to get.
  /// @returns A pointer to the specified tile.
  /// This function never returns null because it
  /// always makes the tile if it doesn't exist.
  virtual Tile* modify_tile(std::size_t x, std::size_t y) = 0;
  /// Accesses the height of the room, in terms of tiles.
  virtual std::size_t get_height() const noexcept { return height; }
  /// Accesses the name of the room.
  virtual const QString& get_name() const noexcept = 0;
  /// Accesses the width of the room, in terms of tiles.
  std::size_t get_width() const noexcept { return width; }
  /// Sets the height of the room.
  virtual void set_height(std::size_t h) { height = h; }
  /// Sets the name of the room.
  virtual void set_name(const QString& name) = 0;
  /// Sets the width of the room.
  virtual void set_width(std::size_t w) { width = w; }
protected:
  /// Constructs a new room base.
  /// @param parent A pointer to the parent object.
  Room(QObject* parent) : QObject(parent), width(5), height(5) {}
};

/// The room table contains all
/// the rooms in the game. Each
/// room is identified by its name.
class RoomTable : public QObject {
public:
  /// Just a stub.
  virtual ~RoomTable() {}
  /// Gets a read-only pointer to a room.
  /// @param index The index to the room to get a pointer of.
  /// @returns On success, a room pointer is returned.
  /// If the index is out of bounds, then a null pointer is returned instead.
  virtual const Room* access_room(std::size_t index) const noexcept = 0;
  /// Creates a new room.
  /// @returns The name of the newly made room.
  virtual QString create_room() = 0;
  /// Gets the name of a room in the room table.
  /// @param index The index of the room to get the name of.
  virtual QString get_name(std::size_t index) const = 0;
  /// Gets a read-write pointer to a room.
  /// @param index The index to the room to get a pointer of.
  /// @returns On success, a room pointer is returned.
  /// If the index is out of bounds, then a null pointer is returned instead.
  virtual Room* modify_room(std::size_t index) noexcept = 0;
  /// Removes a room from the room table.
  /// @param index The index of the room to remove.
  virtual bool remove(std::size_t index) = 0;
  /// Renames a room in the room table.
  /// @param index The index of the room to rename.
  /// @param name The name to give the room.
  /// @returns True on success, false on failure.
  virtual bool rename(std::size_t index, const QString& name) = 0;
  /// Gets the number of rooms in the table.
  virtual std::size_t size() const noexcept = 0;
protected:
  /// Constructs the base room table.
  /// @param parent A pointer to the parent object.
  RoomTable(QObject* parent) : QObject(parent) {}
};

/// A project's data, without any of
/// the extra stuff from the model/view paradigm.
class Project : public QObject {
  Q_OBJECT
public:
  /// Creates a new project model instance.
  /// @param parent A pointer to the parent object.
  /// @returns A pointer to a new project model.
  static ScopedPtr<Project> make(QObject* parent = nullptr);
  /// Just a stub.
  virtual ~Project() {}
  /// Accesses a const-pointer to the room table.
  /// This prevents the modification flag from being set.
  virtual const RoomTable* access_room_table() const noexcept = 0;
  /// Accesses the texture table for reading only
  /// This prevents the modification flag fron being set.
  /// @returns A constant pointer to the texture table.
  virtual const TextureTable* access_texture_table() const noexcept = 0;
  /// Gets a non-const pointer to the room table.
  /// @returns A non-const pointer to the room table.
  virtual RoomTable* modify_room_table() noexcept = 0;
  /// Gets a non-const pointer to the texture table.
  /// @returns A pointer to the model's texture table.
  virtual TextureTable* modify_texture_table() noexcept = 0;
  /// Indicates whether or not the project has been modified.
  /// @returns True if the project has been modified, false otherwise.
  virtual bool is_modified() const noexcept = 0;
  /// Opens a project model.
  /// @param path The path to the model to open.
  /// It's expected that this is a JSON file.
  /// @returns True on success, false on failure.
  virtual bool open(const QString& path) = 0;
  /// Saves the project data.
  /// The data is first saved to a swap file
  /// before replacing the current file.
  /// The modification flag is set to false
  /// after calling this function.
  /// @param path The path to save the data to.
  /// @returns True on success, false on failure.
  virtual bool save(const QString& path) = 0;
protected:
  /// Constructs the base project instance.
  /// @param parent A pointer to the parent object.
  Project(QObject* parent) : QObject(parent) {}
};

} // namespace tk

} // namespace herald
