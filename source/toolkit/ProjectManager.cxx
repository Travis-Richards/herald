#include "ProjectManager.h"

#include "ScopedPtr.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

namespace herald {

namespace tk {

namespace {

/// The JSON game model data.
class JsonModel final {
  /// The root JSON object.
  QJsonObject root_object;
public:
  /// Adds a texture to the model.
  /// @param path The path of the texture to add.
  /// @returns The name assigned to the texture.
  QString add_texture(const QString& path) {

    QFileInfo file_info(path);

    auto textures = root_object["textures"].toArray();

    QJsonObject texture_object;
    texture_object.insert("name", file_info.baseName());
    texture_object.insert("path", path);

    textures.append(texture_object);

    root_object["textures"] = textures;

    return file_info.baseName();
  }
  /// Lists the textures in the model.
  QStringList list_textures() const {

    QStringList texture_names;

    auto textures = root_object["textures"].toArray();

    for (auto texture_value : textures) {

      if (!texture_value.isObject()) {
        continue;
      }

      texture_names << texture_value.toObject()["name"].toString();
    }

    return texture_names;
  }
  /// Opens the model from a file.
  /// @param filename The name of the file containing the JSON file.
  /// @returns True on success, false on failure.
  bool open(const QString& filename) {

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return false;
    }

    root_object = QJsonDocument::fromJson(file.readAll()).object();

    return true;
  }
  /// Saves the model to a JSON file.
  /// @param filename The name of the file to save the data at.
  /// @returns True on success, false on failure.
  bool save(const QString& filename) const {

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      return false;
    }

    file.write(QJsonDocument(root_object).toJson());

    return true;
  }
};

/// This is the implementation of the project manager interface.
class ProjectManagerImpl final : public ProjectManager {
  /// The game directory.
  QDir game_dir;
public:
  /// Adds a new texture to the project.
  /// @param path The path of the texture to open.
  /// @returns The name assigned to the texture.
  QString add_texture(const QString& path) override {

    auto mutator = [path](JsonModel& model) {
      return model.add_texture(path);
    };

    return add_model_item(mutator);
  }
  /// Lists the textures in the project.
  QStringList list_textures() const override {

    auto list_functor = [](const JsonModel& model) {
      return model.list_textures();
    };

    return list_model_items(list_functor);
  }
  /// Opens a project at a certain path.
  /// @param path The path of the project to open.
  /// @returns True on success, false on failure.
  bool open(const QString& path) override {

    game_dir = QDir(path);
    if (!game_dir.exists()) {
      return false;
    }

    return true;
  }
protected:
  /// Lists items from a model.
  /// @param accessor The model accessor function.
  /// @returns A list of items from the model.
  template <typename Accessor>
  QStringList list_model_items(Accessor& accessor) const {

    auto model_path = game_dir.filePath("model.json");

    JsonModel model;

    if (!model.open(model_path)) {
      return QStringList();
    } else {
      return accessor(model);
    }
  };
  /// Adds an item to the model.
  /// @param mutator The function used to add the item.
  /// @returns The name assigned to the model.
  template <typename Mutator>
  QString add_model_item(Mutator mutator) {

    auto model_path = game_dir.filePath("model.json");

    QFileInfo model_info(model_path);

    JsonModel model;

    if (model_info.exists()) {
      if (!model.open(model_path)) {
        return QString();
      }
    }

    auto name = mutator(model);

    model.save(model_path);

    return name;
  }
};

} // namespace

ScopedPtr<ProjectManager> ProjectManager::make() {
  return new ProjectManagerImpl();
}

} // namespace tk

} // namespace herald
