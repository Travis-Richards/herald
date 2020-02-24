#include "ProjectModel.h"

#include <herald/ScopedPtr.h>

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

#include <vector>

#include <QDebug>

namespace herald {

namespace tk {

namespace {

/// A single entry in the texture table.
struct Texture final {
  /// The path to the texture.
  QString path;
  /// The name given to the texture.
  QString name;
  /// Constructs a new entry instance.
  /// @param p The path of the entry.
  Texture(const QString& p) : path(p) {
    QFileInfo file_info(p);
    name = file_info.baseName();
  }
  /// Constructs the texture from a JSON value.
  /// @param json_value The JSON value to get the from.
  /// This should be an object value.
  Texture(const QJsonValue& json_value) {
    auto obj = json_value.toObject();
    path = obj["path"].toString();
    name = obj["name"].toString();
  }
  /// Converts the texture to a JSON value.
  /// @returns The JSON representation of the texture.
  QJsonValue to_json() const {
    QJsonObject object;
    object["name"] = name;
    object["path"] = path;
    return object;
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
  /// Gets the full path of a texture.
  /// @param name The name of the texture to get the path of.
  /// @returns The path of the specified texture.
  QString get_path(const QString& name) const override {

    auto it = find_texture(name);
    if (it == textures.end()) {
      return QString();
    } else {
      return it->path;
    }
  }
  /// Lists the textures in the table.
  /// @returns A list of texture names.
  QStringList list() const override {

    QStringList texture_names;

    for (const auto& texture : textures) {
      texture_names << texture.name;
    }

    return texture_names;
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
  /// @param name The name of the texture to remove.
  void remove(const QString& name) override {
    auto it = find_texture(name);
    if (it != textures.end()) {
      textures.erase(it);
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

/// The implementation of the project model.
/// Uses JSON to store project values.
class ProjectModelImpl final : public ProjectModel {
  /// The texture table for the model.
  TextureTableImpl texture_table;
public:
  /// Accesses a pointer to the texture table.
  /// This function sets the modification flag to true.
  TextureTable* modify_texture_table() override {
    set_modified_flag(true);
    return &texture_table;
  }
  /// Accesses a const-pointer to the texture table.
  const TextureTable* access_texture_table() const noexcept override {
    return &texture_table;
  }
  /// Opens a project model.
  /// @param path The path of the model to open.
  /// @returns True on success, false on failure.
  bool open(const QString& path) override {

    qDebug() << "here";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
      qDebug() << "fail 1";
      return false;
    }

    QJsonParseError parse_error;

    auto doc = QJsonDocument::fromJson(file.readAll(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
      // In the future, we should find a way to display this.
      // For know, we just indicate some sort of failure
      // so that we don't open the project.
      qDebug() << "fail 2";
      return false;
    }

    if (!doc.isObject()) {
      qDebug() << "fail 3";
      return false;
    }

    auto root = doc.object();

    if (!texture_table.read(root["textures"])) {
      return false;
    }

    return true;
  }
  /// Saves the model to a file.
  /// @param path The path to save the model to.
  /// @returns True on success, false on failure.
  bool save(const QString& path) override {

    if (!is_modified()) {
      return true;
    }

    QJsonObject root;
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
