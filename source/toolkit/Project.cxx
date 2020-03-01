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

/// A simple wrapper class around a boolean
/// value that indicates whether or not the project
/// has been modified in any way.
class ModifyFlag final {
  /// Wether or not the project has been modified.
  bool state;
public:
  ModifyFlag(const ModifyFlag&) = delete;
  /// Constructs the mod flag.
  /// The initial value is false.
  constexpr ModifyFlag() noexcept : state(false) {}
  /// Sets the modification state.
  /// @param s The state to assign the modification flag.
  void set(bool s) noexcept {
    state = s;
  }
  /// Indicates whether or not there has been modification.
  operator bool () const noexcept {
    return state;
  }
};

/// A single entry in the texture table.
struct Texture final {
  /// The name given to the texture.
  QString name;
  /// The path that the image originated from.
  QString path;
  /// The image data of the texture.
  QByteArray data;
  /// The modification flag.
  ModifyFlag* mod_flag;
  /// Constructs a new entry instance.
  /// @param p The path of the entry.
  Texture(const QString& p, ModifyFlag* mf) : name(QFileInfo(p).baseName()), path(p), mod_flag(mf) {
    read_data(path);
  }
  /// Constructs the texture from a JSON value.
  /// @param json_value The JSON value to get the from.
  /// This should be an object value.
  Texture(const QJsonValue& json_value, ModifyFlag* mf) : mod_flag(mf) {
    auto obj = json_value.toObject();
    name = obj["name"].toString();
    path = obj["path"].toString();
    data = QByteArray::fromBase64(obj["data"].toString().toUtf8());
  }
  /// Converts the texture to a JSON value.
  /// @returns The JSON representation of the texture.
  QJsonValue to_json() const {
    QJsonObject object;
    object["name"] = name;
    object["path"] = path;
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
  std::vector<ScopedPtr<Texture>> textures;
  /// The project modification flag.
  ModifyFlag* mod_flag;
public:
  /// Constructs a new instance of the texture table.
  /// @param mf The project modification flag.
  /// @param parent A pointer to the parent object.
  TextureTableImpl(ModifyFlag* mf, QObject* parent) : TextureTable(parent), mod_flag(mf) {}
  /// Adds a texture to the table.
  /// @param path The path of the texture to add.
  /// @returns The name assigned to the texture.
  QString add(const QString& path) override {

    mod_flag->set(true);

    textures.emplace_back(ScopedPtr<Texture>::make(path, mod_flag));

    return textures[textures.size() - 1]->name;
  }
  QByteArray find_texture_data(const QString& name) const override {

    for (const auto& texture : textures) {
      if (texture->name == name) {
        return texture->data;
      }
    }

    return QByteArray();
  }
  /// Accesses the texture image data.
  /// @param index The index of the texture to get the data for.
  /// @returns The image data of the specified texture.
  /// If the index is out of bounds, then an empty byte array is returned.
  QByteArray get_data(std::size_t index) const override {
    if (index >= textures.size()) {
      return QByteArray();
    } else {
      return textures[index]->data;
    }
  }
  /// Gets the name of a texture.
  /// @param index The index of the texture to get the name of.
  QString get_name(std::size_t index) const override {
    if (index >= textures.size()) {
      return QString();
    } else {
      return textures[index]->name;
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
      textures.emplace_back(ScopedPtr<Texture>::make(texture, mod_flag));
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
      mod_flag->set(true);
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

    mod_flag->set(true);

    textures[index]->name = name;

    return true;
  }
  /// Converts the texture table to a JSON value.
  /// @returns The JSON value containing the texture table.
  QJsonValue to_json() const {

    QJsonArray json_array;

    for (const auto& texture : textures) {
      json_array.append(texture->to_json());
    }

    return json_array;
  }
  /// Indicates the number of textures in the texture table.
  std::size_t size() const noexcept override {
    return textures.size();
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
  /// The project modification flag.
  ModifyFlag* mod_flag;
public:
  /// Reads a tile from a JSON value.
  /// @param json_value The JSON value to read the tile data from.
  /// @param parent A pointer to the parent object.
  TileImpl(const QJsonValue& json_value, ModifyFlag* mf, QObject* parent) : Tile(parent), mod_flag(mf) {
    auto json_object = json_value.toObject();
    texture_name = json_object["texture"].toString();
    x = (std::size_t) json_object["x"].toInt(1);
    y = (std::size_t) json_object["y"].toInt(1);
  }
  /// Constructs a new tile instance.
  /// @param x_ The X coordinate to assign the tile.
  /// @param y_ The Y coordinate to assign the tile.
  /// @param parent A pointer to the parent object.
  TileImpl(std::size_t x_, std::size_t y_, ModifyFlag* mf, QObject* parent)
    : Tile(parent), x(x_), y(y_), mod_flag(mf) {}
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
    mod_flag->set(true);
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
  /// The project modification flag.
  ModifyFlag* mod_flag;
public:
  /// Constructs a new room instance.
  /// @param name_ The name to give the room.
  /// @param mf The project modification flag.
  /// @param parent A pointer to the parent object.
  RoomImpl(const QString& name_, ModifyFlag* mf, QObject* parent) : Room(parent), name(name_), mod_flag(mf) { }
  /// Constructs a room from a JSON value.
  /// @param mf The project modification flag.
  /// @param room The JSON room to get the data from.
  RoomImpl(const QJsonValue& room, ModifyFlag* mf, QObject* parent) : Room(parent), mod_flag(mf) {

    auto room_object = room.toObject();

    name = room_object["name"].toString();
    set_width((std::size_t) room_object["width"].toInt(1));
    set_height((std::size_t) room_object["height"].toInt(1));

    auto json_tiles = room_object["tiles"].toArray();
    for (auto json_tile : json_tiles) {
      tiles.emplace_back(ScopedPtr<TileImpl>::make(json_tile, mod_flag, this));
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

    tiles.emplace_back(new TileImpl(x, y, mod_flag, this));

    return tiles[tiles.size() - 1].get();
  }
  /// Sets the height of the room.
  /// @param h The height to assign to the room.
  void set_height(std::size_t h) override {
    mod_flag->set(true);
    Room::set_height(h);
  }
  /// Modifies the name of the room.
  void set_name(const QString& name_) override {
    mod_flag->set(true);
    name = name_;
  }
  /// Sets the width of the room.
  /// @param w The width to assign the room.
  void set_width(std::size_t w) override {
    mod_flag->set(true);
    Room::set_width(w);
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
};

/// This is the implementation for the room table.
class RoomTableImpl final : public RoomTable {
  /// The rooms part of the room table.
  std::vector<ScopedPtr<RoomImpl>> rooms;
  /// The project modification flag.
  ModifyFlag* mod_flag;
public:
  /// Constructs a new instance of the room table.
  /// @param mf The project modification flag.
  /// @param parent A pointer to the parent object.
  RoomTableImpl(ModifyFlag* mf, QObject* parent) : RoomTable(parent), mod_flag(mf) {}
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

    mod_flag->set(true);

    QString basename = "New Room";

    QString name = basename;

    for (int i = 0; i < INT_MAX; i++) {
      if (is_unique(name)) {
        rooms.emplace_back(ScopedPtr<RoomImpl>::make(name, mod_flag, this));
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
        rooms.emplace_back(ScopedPtr<RoomImpl>::make(json_room, mod_flag, this));
      }
    }

    return true;
  }
  /// Removes a room from the room table.
  bool remove(std::size_t index) override {
    if (index >= rooms.size()) {
      return false;
    } else {
      mod_flag->set(true);
      rooms.erase(rooms.begin() + index);
      return true;
    }
  }
  /// Renames a room.
  bool rename(std::size_t index, const QString& name) override {
    if (index >= rooms.size()) {
      return false;
    } else {
      mod_flag->set(true);
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
  /// The project modification flag.
  ModifyFlag mod_flag;
public:
  /// Constructs a new project instance.
  /// @param parent A pointer to the parent object.
  ProjectImpl(QObject* parent)
    : Project(parent),
      room_table(&mod_flag, this),
      texture_table(&mod_flag, this) {

  }
  /// Accesses a const-pointer to the room table.
  const RoomTable* access_room_table() const noexcept override {
    return &room_table;
  }
  /// Accesses a const-pointer to the texture table.
  const TextureTable* access_texture_table() const noexcept override {
    return &texture_table;
  }
  /// Indicates whether or not the project has been modified.
  /// @returns True if there's been modification, false otherwise.
  bool is_modified() const noexcept override {
    return mod_flag;
  }
  /// Accesses a pointer to the room table.
  /// This function also sets the modification flag to true.
  RoomTable* modify_room_table() noexcept override {
    return &room_table;
  }
  /// Accesses a pointer to the texture table.
  /// This function sets the modification flag to true.
  TextureTable* modify_texture_table() noexcept override {
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
      mod_flag.set(false);
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
