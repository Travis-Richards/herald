#include "ProjectModel.h"

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

/// A single entry in the room table.
class RoomImpl final : public Room {
public:
  /// The name of the room.
  QString name;
  /// Constructs a new room instance.
  /// @param name_ The name to give the room.
  RoomImpl(const QString& name_) : name(name_) {
  }
  /// Constructs a room from a JSON value.
  /// @param room The JSON room to get the data from.
  RoomImpl(const QJsonValue& room) {

    auto room_object = room.toObject();

    name = room_object["name"].toString();

    set_width((std::size_t) room_object["width"].toInt(1));

    set_height((std::size_t) room_object["height"].toInt(1));
  }
  /// Accesses the name of the room.
  const QString& get_name() const noexcept override {
    return name;
  }
  /// Converts the room to a JSON value.
  QJsonValue to_json() const {
    QJsonObject object;
    object["name"] = name;
    object["width"] = (int) get_width();
    object["height"] = (int) get_height();
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
  std::vector<RoomImpl> rooms;
public:
  /// Accesses a room for reading.
  const Room* access_room(std::size_t index) const noexcept override {
    if (index >= rooms.size()) {
      return nullptr;
    } else {
      return &rooms[index];
    }
  }
  /// Creates a new room, with a unique name.
  /// @returns The name of the newly created room.
  QString create_room() override {

    QString basename = "New Room";

    QString name = basename;

    for (int i = 0; i < INT_MAX; i++) {
      if (is_unique(name)) {
        rooms.emplace_back(name);
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
      return rooms[index].name;
    }
  }
  /// Accesses a room for modification.
  Room* modify_room(std::size_t index) noexcept override {
    if (index >= rooms.size()) {
      return nullptr;
    } else {
      return &rooms[index];
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
        rooms.emplace_back(json_room);
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
      rooms[index].name = name;
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
      json_array.append(room.to_json());
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
      if (room.name == name) {
        return false;
      }
    }

    return true;
  }
};

/// The implementation of the project model.
/// Uses JSON to store project values.
class ProjectModelImpl final : public ProjectModel {
  /// The room table for the model.
  RoomTableImpl room_table;
  /// The texture table for the model.
  TextureTableImpl texture_table;
public:
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
  RoomTable* modify_room_table() override {
    set_modified_flag(true);
    return &room_table;
  }
  /// Accesses a pointer to the texture table.
  /// This function sets the modification flag to true.
  TextureTable* modify_texture_table() override {
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

ScopedPtr<ProjectModel> ProjectModel::make() {
  return new ProjectModelImpl;
}

} // namespace tk

} // namespace herald
