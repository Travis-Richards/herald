#include "ProjectManager.h"

#include <herald/ScopedPtr.h>

#include "GameInfo.h"
#include "Language.h"
#include "SourceTreeModel.h"

#include <QDir>
#include <QFile>
#include <QFileSystemModel>
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
  /// Removes a texture from the model.
  /// @param name The name of the texture to delete.
  void delete_texture(const QString& name) {

    auto textures = root_object["textures"].toArray();

    QJsonArray next_textures;

    for (auto texture : textures) {

      auto texture_obj = texture.toObject();

      if (texture_obj["name"].toString() != name) {
        next_textures.append(texture);
      }
    }

    root_object["textures"] = next_textures;
  }
  /// Locates a texture by its name.
  /// @param name The name of the texture to find.
  /// @returns The value for the specified texture.
  QJsonValue find_texture(const QString& name) const {

    auto textures = root_object["textures"].toArray();

    for (auto texture_value : textures) {

      auto texture_obj = texture_value.toObject();

      if (texture_obj["name"].toString() == name) {
        return texture_value;
      }
    }

    return QJsonValue();
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
  /// Renames a texture.
  /// @param index The index of the texture to rename.
  /// @param name The name to give the texture.
  void rename_texture(std::size_t index, const QString& name) {

    auto textures = root_object["textures"].toArray();

    auto texture_object = textures[index].toObject();

    texture_object["name"] = name;

    textures[index] = texture_object;

    root_object["textures"] = textures;
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
  /// Gets the full path of a texture.
  /// @param name The name of the texture to get the path of.
  /// @returns The path of the specified texture.
  QString texture_path(const QString& name) const {
    return find_texture(name).toObject()["path"].toString();
  }
};

/// This is the implementation of the project manager interface.
class ProjectManagerImpl final : public ProjectManager {
  /// The game directory.
  QDir game_dir;
  /// The model for the project's source tree.
  ScopedPtr<SourceTreeModel> source_tree;
  /// The language used by the project.
  ScopedPtr<Language> language;
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
  /// Removes a texture from the project.
  /// @param name The name of the texture to remove.
  void delete_texture(const QString& name) override {

    auto texture_deleter = [name](JsonModel& model) {
      model.delete_texture(name);
    };

    modify_model(texture_deleter);
  }
  /// Accesses a pointer to the project language.
  Language* get_language() noexcept override {
    return language.get();
  }
  /// Accesses a pointer to the source manager.
  /// @returns A pointer to the source tree manager.
  SourceTreeModel* get_source_tree_model() noexcept override {
    return source_tree.get();
  }
  /// Indicates whether or not there are unsaved changes in the project.
  /// @returns True if there's unsaved changes, false otherwise.
  bool has_unsaved_changes() const override {
    return false;
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

    source_tree = SourceTreeModel::make(game_dir.filePath("source"));

    auto info = open_info();

    language = Language::make_from_name(info->api());

    return true;
  }
  /// Opens the game information.
  ScopedPtr<GameInfo> open_info() const override {
    return GameInfo::open(game_dir.absolutePath());
  }
  /// Renames a texture.
  /// @param index The index of the texture to rename.
  /// @param name The name to give to the texture.
  void rename_texture(std::size_t index, const QString& name) override {

    auto mutator = [index, name](JsonModel& model) {
      model.rename_texture(index, name);
    };

    modify_model(mutator);
  }
  /// Saves all project data.
  /// @returns True on success, false on failure.
  bool save_all() override {
    return true;
  }
  /// Gets the path to a texture file.
  /// @param name The name of the texture to get the path of.
  /// @returns The path to the specified texture.
  QString texture_path(const QString& name) override {

    JsonModel model;

    if (!model.open(get_model_path())) {
      return "";
    }

    return model.texture_path(name);
  }
protected:
  /// Gets the path to the model.
  /// @returns The path to the model.
  QString get_model_path() const {
    return game_dir.filePath("model.json");
  }
  /// Adds an item to the model.
  /// @param mutator The function used to add the item.
  /// @returns The name assigned to the model.
  template <typename Mutator>
  QString add_model_item(Mutator mutator) {

    auto model_path = get_model_path();

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
  /// Lists items from a model.
  /// @param accessor The model accessor function.
  /// @returns A list of items from the model.
  template <typename Accessor>
  QStringList list_model_items(Accessor& accessor) const {

    JsonModel model;

    if (!model.open(get_model_path())) {
      return QStringList();
    } else {
      return accessor(model);
    }
  };
  /// Modifies the model.
  /// @param mutator The function used to modify the model.
  /// @returns True on success, false on failure.
  template <typename Mutator>
  bool modify_model(Mutator mutator) {

    auto model_path = get_model_path();

    QFileInfo model_info(model_path);

    JsonModel model;

    if (model_info.exists()) {
      if (!model.open(model_path)) {
        return false;
      }
    }

    mutator(model);

    model.save(model_path);

    return true;
  }
};

} // namespace

ScopedPtr<ProjectManager> ProjectManager::make() {
  return new ProjectManagerImpl();
}

} // namespace tk

} // namespace herald
