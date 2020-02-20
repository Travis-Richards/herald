#include "LegacyModelLoader.h"

#include <herald/Action.h>
#include <herald/ActionTable.h>
#include <herald/Animation.h>
#include <herald/AnimationTable.h>
#include <herald/Engine.h>
#include <herald/Index.h>
#include <herald/Model.h>
#include <herald/ScopedPtr.h>
#include <herald/TextureTable.h>

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace herald {

namespace {

/// Used for loading a legacy game model.
class LegacyModelLoader final {
  /// The root directory.
  QDir root;
  /// The model to put the data into.
  Model* model;
public:
  /// Constructs the legacy model loader.
  /// @param m The model to put the data into.
  /// @param game_path The path of the game to open.
  LegacyModelLoader(Model* m, const QString& game_path)
    : root(game_path), model(m) {

  }
  /// Indicates whether or not the loader directory is valid.
  /// @returns True if it's valid, false otherwise.
  bool valid() {
    return root.exists();
  }
  /// Loads the model.
  /// @returns True on success, false on failure.
  bool load() {
    return load_textures()
        && load_actions();
  }
protected:
  /// Loads the actions file.
  /// @returns True on success, false on failure.
  bool load_actions() {

    QFile actions_file(root.filePath("actions.json"));

    if (!actions_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return false;
    }

    auto actions_doc = QJsonDocument::fromJson(actions_file.readAll());

    auto actions_array = actions_doc.array();

    auto* action_table = model->get_action_table();

    for (auto action : actions_array) {

      auto animation = action.toObject()["animation"].toInt(INT_MAX);

      action_table->add(Action((std::size_t) animation));
    }

    return true;
  }
  /// Loads textures into the model.
  /// @returns True on success, false on failure.
  bool load_textures() {

    QDir::Filters filters = QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Dirs;

    auto texture_list = get_sorted_dir_entries(root.filePath("textures"), filters);

    auto* texture_table = model->get_texture_table();

    auto* animation_table = model->get_animation_table();

    for (auto texture : texture_list) {

      QFileInfo texture_info(texture);

      if (texture_info.isDir()) {
        load_animation_dir(texture);
      } else {
        // Ensure these two functions
        // are kept in this order.
        animation_table->add_still_frame(texture_table->size());
        texture_table->open(texture.toStdString().c_str());
      }
    }

    return true;
  }
  /// Loads a directory containing textures that make up an animation.
  /// @param path The path to the animation directory.
  /// @returns True on success, false on failure.
  bool load_animation_dir(const QString& path) {

    auto* texture_table = model->get_texture_table();

    auto* animation_table = model->get_animation_table();

    auto texture_offset = texture_table->size();

    // Default 30fps
    std::size_t delay = 1000 / 30;

    auto animation = Animation::make();

    QDir::Filters filters = QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files;

    auto textures = get_sorted_dir_entries(path, filters);

    for (auto texture_path : textures) {

      animation->add_frame(texture_offset++, delay);

      texture_table->open(texture_path.toStdString().c_str());
    }

    animation_table->add(std::move(animation));

    return true;
  }
  /// Accumulates the paths of a directory
  /// and sorts them into a string list.
  /// @param path The path to get the entries of.
  /// @param filters The filters to apply to the entries.
  /// @returns A list of files and or directories from the specified path.
  QStringList get_sorted_dir_entries(const QString& path, QDir::Filters filters) {

    QDirIterator dir_iterator(path, filters);

    QStringList entry_list;

    while (dir_iterator.hasNext()) {
      entry_list << dir_iterator.next();
    }

    entry_list.sort();

    return entry_list;
  }
};

} // namespace

bool load_legacy_model(Model* model, const QString& game_path) {

  LegacyModelLoader loader(model, game_path);

  if (!loader.valid()) {
    return false;
  }

  return loader.load();
}

} // namespace herald
