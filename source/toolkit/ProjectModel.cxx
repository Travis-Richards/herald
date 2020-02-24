#include "ProjectModel.h"

#include <herald/ScopedPtr.h>

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace herald {

namespace tk {

namespace {

/// The implementation of the texture table.
class TextureTableImpl final : public TextureTable {
  /// The array of textures.
  QJsonArray texture_array;
public:
  /// Adds a texture to the table.
  /// @param path The path of the texture to add.
  /// @returns The name assigned to the texture.
  QString add(const QString& path) override {

    QFileInfo file_info(path);

    QJsonObject texture_object;
    texture_object.insert("name", file_info.baseName());
    texture_object.insert("path", path);

    texture_array.append(texture_object);

    // TODO : Generate unique name
    // if basename exists already.

    return file_info.baseName();
  }
  /// Reads a texture table from a JSON value.
  /// @param value The JSON value to read the textures from.
  /// @returns True on success, false on failure.
  bool read(const QJsonValue& value) {

    if (!value.isArray()) {
      return false;
    }

    // TODO : further validation

    texture_array = value.toArray();

    return true;
  }
  /// Removes a texture from the model.
  /// @param name The name of the texture to remove.
  void remove(const QString& name) override {
    auto index = find_texture(name);
    if (index >= 0) {
      texture_array.removeAt(index);
    }
  }
  /// Lists the textures in the table.
  /// @returns A list of texture names.
  QStringList list() const override {

    QStringList texture_names;

    for (auto texture_value : texture_array) {

      auto name = texture_value.toObject()["name"].toString();
      if (!name.isEmpty()) {
        texture_names << name;
      }
    }

    return texture_names;
  }
  /// Renames a texture.
  /// @param index The index of the texture to rename.
  /// @param name The name to give the texture.
  /// @returns True on success, false on failure.
  bool rename(std::size_t index, const QString& name) override {

    if (index >= ((std::size_t) texture_array.size())) {
      return false;
    }

    auto texture_object = texture_array[(int) index].toObject();

    texture_object["name"] = name;

    texture_array[(int) index] = texture_object;

    return true;
  }
  /// Gets the full path of a texture.
  /// @param name The name of the texture to get the path of.
  /// @returns The path of the specified texture.
  QString get_path(const QString& name) const override {

    auto index = find_texture(name);
    if (index < 0) {
      return QString();
    }

    return texture_array[index].toObject()["path"].toString();
  }
protected:
  /// Locates a texture in the table.
  /// @param name The name of the texture to search for.
  /// @returns The index of the texture.
  /// If the texture wasn't found, then a negative one is returned.
  int find_texture(const QString& name) const {

    for (int i = 0; i < texture_array.size(); i++) {

      auto texture_obj = texture_array[i].toObject();

      if (texture_obj["name"].toString() == name) {
        return i;
      }
    }

    return -1;
  }
};

/// The implementation of the project model.
/// Uses JSON to store project values.
class ProjectModelImpl final : public ProjectModel {
  /// The root JSON object for the project.
  QJsonObject root;
  /// The texture table for the model.
  TextureTableImpl texture_table;
public:
  /// Accesses a pointer to the texture table.
  TextureTable* get_texture_table() override {
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

    root = doc.object();

    if (!texture_table.read(root["textures"])) {
      return false;
    }

    return true;
  }
};

} // namespace

ScopedPtr<ProjectModel> ProjectModel::make() {
  return new ProjectModelImpl;
}

} // namespace tk

} // namespace herald
