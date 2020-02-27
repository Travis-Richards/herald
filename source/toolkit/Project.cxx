#include "Project.h"

#include <herald/ScopedPtr.h>

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

#include <vector>

namespace herald {

namespace tk {

namespace {

/// A single entry in the texture table.
struct Texture final {
  /// The name given to the texture.
  QString name;
  /// The image data of the texture.
  QByteArray data;
  /// Constructs a new entry instance.
  /// @param path The path of the entry.
  Texture(const QString& path) : name(QFileInfo(path).baseName()) {
    read_data(path);
  }
  /// Constructs the texture from a JSON value.
  /// @param json_value The JSON value to get the from.
  /// This should be an object value.
  Texture(const QJsonValue& json_value) {
    auto obj = json_value.toObject();
    name = obj["name"].toString();
    data = QByteArray::fromBase64(obj["data"].toString().toUtf8());
  }
  /// Converts the texture to a JSON value.
  /// @returns The JSON representation of the texture.
  QJsonValue to_json() const {
    QJsonObject object;
    object["name"] = name;
    object["data"] = QString(data.toBase64());
    return object;
  }
protected:
  /// Reads the file data.
  /// @param path The path to get the data from.
  /// @returns True on success, false on failure.
  bool read_data(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
      return false;
    } else {
      data = file.readAll();
      return true;
    }
  }
};

/// The implementation of the texture table.
class TextureTableImpl final : public TextureTable {
  /// The entries made into the texture table.
  std::vector<Texture> textures;
public:
  /// Constructs a new instance of the texture table.
  /// @param parent A pointer to the parent object.
  TextureTableImpl(QObject* parent) : TextureTable(parent) {}
  /// Adds a texture to the table.
  /// @param path The path of the texture to add.
  /// @returns The name assigned to the texture.
  QString add(const QString& path) override {
    textures.emplace_back(path);
    return textures[textures.size() - 1].name;
  }
  /// Accesses the texture image data.
  /// @param index The index of the texture to get the data for.
  /// @returns The image data of the specified texture.
  /// If the index is out of bounds, then an empty byte array is returned.
  QByteArray get_data(std::size_t index) const override {
    if (index >= textures.size()) {
      return QByteArray();
    } else {
      return textures[index].data;
    }
  }
  /// Gets the name of a texture.
  /// @param index The index of the texture to get the name of.
  QString get_name(std::size_t index) const override {
    if (index >= textures.size()) {
      return QString();
    } else {
      return textures[index].name;
    }
  }
  /// Reads a texture table from a JSON value.
  /// @param value The JSON value to read the textures from.
  /// @returns True on success, false on failure.
  bool read(const QJsonValue& value) {

    if (!value.isArray()) {
      return false;
    }

    for (auto texture : value.toArray()) {
      textures.emplace_back(texture);
    }

    return true;
  }
  /// Removes a texture from the model.
  /// @param index The index of the item to remove.
  /// @returns True on success, false on failure.
  bool remove(std::size_t index) override {
    if (index >= textures.size()) {
      return false;
    } else {
      textures.erase(textures.begin() + index);
      return true;
    }
  }
  /// Renames a texture.
  /// @param index The index of the texture to rename.
  /// @param name The name to give the texture.
  /// @returns True on success, false on failure.
  bool rename(std::size_t index, const QString& name) override {

    if (index >= textures.size()) {
      return false;
    }

    textures[index].name = name;

    return true;
  }
  /// Converts the texture table to a JSON value.
  /// @returns The JSON value containing the texture table.
  QJsonValue to_json() const {

    QJsonArray json_array;

    for (const auto& texture : textures) {
      json_array.append(texture.to_json());
    }

    return json_array;
  }
  /// Indicates the number of textures in the texture table.
  std::size_t size() const noexcept override {
    return textures.size();
  }
protected:
  /// Locates a texture in the table.
  /// @param name The name of the texture to search for.
  /// @returns The index of the texture.
  /// If the texture wasn't found, then a negative one is returned.
  std::vector<Texture>::const_iterator find_texture(const QString& name) const {

    for (auto it = textures.begin(); it != textures.end(); it++) {
      if (it->name == name) {
        return it;
      }
    }

    return textures.end();
  }
};

/// The implementation of the tile interface.
class TileImpl final : public Tile {
  /// The name of the texture being displayed.
  QString texture_name;
  /// The X coordinate of this tile.
  std::size_t x;
  /// The Y coordinate of this tile.
  std::size_t y;
public:
  /// Reads a tile from a JSON value.
  /// @param json_value The JSON value to read the tile data from.
  /// @param parent A pointer to the parent object.
  TileImpl(const QJsonValue& json_value, QObject* parent) : Tile(parent) {
    auto json_object = json_value.toObject();
    texture_name = json_object["name"].toString();
    x = (std::size_t) json_object["x"].toInt(1);
    y = (std::size_t) json_object["y"].toInt(1);
  }
  /// Constructs a new tile instance.
  /// @param x_ The X coordinate to assign the tile.
  /// @param y_ The Y coordinate to assign the tile.
  /// @param parent A pointer to the parent object.
  TileImpl(std::size_t x_, std::size_t y_, QObject* parent) : Tile(parent), x(x_), y(y_) {}
  /// Accesses the name of the texture being displayed.
  /// @returns The name of the texture.
  const QString& get_texture() const noexcept override {
    return texture_name;
  }
  /// Indicates if this tile has a certain set of coordinates.
  /// @param x_ The X coordinate to check for.
  /// @param y_ The Y coordinate to check for.
  /// @returns True if this tile has the specified coordinates, false otherwise.
  bool has_coordinates(std::size_t x_, std::size_t y_) const noexcept {
    return (x == x_)
        && (y == y_);
  }
  /// Sets the name of the texture being displayed.
  /// @param name The name to assign.
  void set_texture(const QString& name) override {
    texture_name = name;
  }
  /// Converts the tile to a JSON value.
  /// @returns The JSON data for this tile.
  QJsonValue to_json() const {
    QJsonObject json_object;
    json_object["texture"] = texture_name;
    json_object["x"] = (int) x;
    json_object["y"] = (int) y;
    return json_object;
  }
};

/// A single entry in the room table.
class RoomImpl final : public Room {
  /// The room tiles.
  std::vector<ScopedPtr<TileImpl>> tiles;
  /// The name of the room.
  QString name;
public:
  /// Constructs a new room instance.
  /// @param name_ The name to give the room.
  /// @param parent A pointer to the parent object.
  RoomImpl(const QString& name_, QObject* parent) : Room(parent), name(name_) { }
  /// Constructs a room from a JSON value.
  /// @param room The JSON room to get the data from.
  RoomImpl(const QJsonValue& room, QObject* parent) : Room(parent) {

    auto room_object = room.toObject();

    name = room_object["name"].toString();
    set_width((std::size_t) room_object["width"].toInt(1));
    set_height((std::size_t) room_object["height"].toInt(1));

    auto json_tiles = room_object["tiles"].toArray();
    for (auto json_tile : json_tiles) {
      tiles.emplace_back(ScopedPtr<TileImpl>::make(json_tile, this));
    }
  }
  /// Accesses a tile for reading.
  /// @param x The X coordinate of the tile.
  /// @param y The Y coordinate of the tile.
  const Tile* access_tile(std::size_t x, std::size_t y) const noexcept override {
    for (const auto& tile : tiles) {
      if (tile->has_coordinates(x, y)) {
        return tile.get();
      }
    }
    return nullptr;
  }
  /// Accesses the name of the room.
  const QString& get_name() const noexcept override {
    return name;
  }
  /// Indicates if the room has a certain name.
  /// @param other_name The name to check for.
  bool has_name(const QString& other_name) const {
    return name == other_name;
  }
  /// Gets a tile for modification.
  /// @param x The X coordinate of the tile to get.
  /// @param y The Y coordinate of the tile to get.
  /// @returns A pointer to the specified tile.
  Tile* modify_tile(std::size_t x, std::size_t y) override {

    for (auto& tile : tiles) {
      if (tile->has_coordinates(x, y)) {
        return tile.get();
      }
    }

    tiles.emplace_back(new TileImpl(x, y, this));

    return tiles[tiles.size() - 1].get();
  }
  /// Converts the room to a JSON value.
  QJsonValue to_json() const {

    QJsonArray json_tiles;

    for (const auto& tile : tiles) {
      json_tiles.append(tile->to_json());
    }

    QJsonObject object;
    object["name"] = name;
    object["width"] = (int) get_width();
    object["height"] = (int) get_height();
    object["tiles"] = json_tiles;

    return object;
  }
  /// Modifies the name of the room.
  void set_name(const QString& name_) override {
    name = name_;
  }
};

/// This is the implementation for the room table.
class RoomTableImpl final : public RoomTable {
  /// The rooms part of the room table.
  std::vector<ScopedPtr<RoomImpl>> rooms;
public:
  /// Constructs a new instance of the room table.
  /// @param parent A pointer to the parent object.
  RoomTableImpl(QObject* parent) : RoomTable(parent) {}
  /// Accesses a room for reading.
  const Room* access_room(std::size_t index) const noexcept override {
    if (index >= rooms.size()) {
      return nullptr;
    } else {
      return rooms[index].get();
    }
  }
  /// Creates a new room, with a unique name.
  /// @returns The name of the newly created room.
  QString create_room() override {

    QString basename = "New Room";

    QString name = basename;

    for (int i = 0; i < INT_MAX; i++) {
      if (is_unique(name)) {
        rooms.emplace_back(ScopedPtr<RoomImpl>::make(name, this));
        return name;
      } else {
        name = basename + " (" + QString::number(i + 1) + ")";
      }
    }

    return QString();
  }
  /// Gets the name of a room in the room table.
  /// @param index The index of the room to get the name of.
  QString get_name(std::size_t index) const override {
    if (index >= rooms.size()) {
      return QString();
    } else {
      return rooms[index]->get_name();
    }
  }
  /// Accesses a room for modification.
  Room* modify_room(std::size_t index) noexcept override {
    if (index >= rooms.size()) {
      return nullptr;
    } else {
      return rooms[index].get();
    }
  }
  /// Reads room table data from a JSON value.
  /// @param json_value The JSON value to read the data from.
  /// @returns True on success, false on failure.
  bool read(const QJsonValue& json_value) {

    if (!json_value.isArray()) {
      return false;
    }

    for (auto json_room : json_value.toArray()) {
      if (!json_room.isObject()) {
        return false;
      } else {
        rooms.emplace_back(ScopedPtr<RoomImpl>::make(json_room, this));
      }
    }

    return true;
  }
  /// Removes a room from the room table.
  bool remove(std::size_t index) override {
    if (index >= rooms.size()) {
      return false;
    } else {
      rooms.erase(rooms.begin() + index);
      return true;
    }
  }
  /// Renames a room.
  bool rename(std::size_t index, const QString& name) override {
    if (index >= rooms.size()) {
      return false;
    } else {
      rooms[index]->set_name(name);
      return true;
    }
  }
  /// Gets the size of the room table.
  /// @returns The number of rooms in the table.
  std::size_t size() const noexcept override {
    return rooms.size();
  }
  /// Converts the room table to a JSON value.
  QJsonValue to_json() const {

    QJsonArray json_array;

    for (const auto& room : rooms) {
      json_array.append(room->to_json());
    }

    return json_array;
  }
protected:
  /// Indicates if a name is unique or not.
  /// @param name The name to check for.
  /// @returns True if any of the rooms
  /// have a name equal to @p name, false otherwise
  bool is_unique(const QString& name) {

    for (const auto& room : rooms) {
      if (room->has_name(name)) {
        return false;
      }
    }

    return true;
  }
};

/// The implementation of the project model.
/// Uses JSON to store project values.
class ProjectImpl final : public Project {
  /// The room table for the model.
  RoomTableImpl room_table;
  /// The texture table for the model.
  TextureTableImpl texture_table;
public:
  /// Constructs a new project instance.
  /// @param parent A pointer to the parent object.
  ProjectImpl(QObject* parent)
    : Project(parent),
      room_table(this),
      texture_table(this) {

  }
  /// Accesses a const-pointer to the room table.
  const RoomTable* access_room_table() const noexcept override {
    return &room_table;
  }
  /// Accesses a const-pointer to the texture table.
  const TextureTable* access_texture_table() const noexcept override {
    return &texture_table;
  }
  /// Accesses a pointer to the room table.
  /// This function also sets the modification flag to true.
  RoomTable* modify_room_table() noexcept override {
    set_modified_flag(true);
    return &room_table;
  }
  /// Accesses a pointer to the texture table.
  /// This function sets the modification flag to true.
  TextureTable* modify_texture_table() noexcept override {
    set_modified_flag(true);
    return &texture_table;
  }
  /// Opens a project model.
  /// @param path The path of the model to open.
  /// @returns True on success, false on failure.
  bool open(const QString& path) override {

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
      return false;
    }

    QJsonParseError parse_error;

    auto doc = QJsonDocument::fromJson(file.readAll(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
      // In the future, we should find a way to display this.
      // For know, we just indicate some sort of failure
      // so that we don't open the project.
      return false;
    }

    if (!doc.isObject()) {
      return false;
    }

    auto root = doc.object();

    return texture_table.read(root["textures"])
           && room_table.read(root["rooms"]);
  }
  /// Saves the model to a file.
  /// @param path The path to save the model to.
  /// @returns True on success, false on failure.
  bool save(const QString& path) override {

    if (!is_modified()) {
      return true;
    }

    QJsonObject root;
    root["rooms"] = room_table.to_json();
    root["textures"] = texture_table.to_json();

    QJsonDocument doc(root);

    QSaveFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
      return false;
    }

    file.write(doc.toJson());

    if (file.commit()) {
      set_modified_flag(false);
      return true;
    } else {
      return false;
    }
  }
};

} // namespace

ScopedPtr<Project> Project::make(QObject* parent) {
  return new ProjectImpl(parent);
}

} // namespace tk

} // namespace herald
