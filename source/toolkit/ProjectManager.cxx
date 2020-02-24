#include "ProjectManager.h"

#include <herald/ScopedPtr.h>

#include "GameInfo.h"
#include "Language.h"
#include "ProjectModel.h"
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

/// This is the implementation of the project manager interface.
class ProjectManagerImpl final : public ProjectManager {
  /// The game directory.
  QDir game_dir;
  /// The model for the project's source tree.
  ScopedPtr<SourceTreeModel> source_tree;
  /// The data model for the project.
  ScopedPtr<ProjectModel> project_model;
  /// The language used by the project.
  ScopedPtr<Language> language;
public:
  /// Constructs a new instance of the project manager.
  ProjectManagerImpl() {
    project_model = ProjectModel::make();
  }
  /// Accesses a pointer to the project language.
  Language* get_language() noexcept override {
    return language.get();
  }
  /// Accesses a pointer to the project model.
  ProjectModel* get_model() noexcept override {
    return project_model.get();
  }
  /// Accesses a pointer to the source manager.
  /// @returns A pointer to the source tree manager.
  SourceTreeModel* get_source_tree_model() noexcept override {
    return source_tree.get();
  }
  /// Indicates whether or not there are unsaved changes in the project.
  /// @returns True if there's unsaved changes, false otherwise.
  bool has_unsaved_changes() const override {
    return project_model->is_modified();
  }
  /// Opens a project at a certain path.
  /// @param path The path of the project to open.
  /// @returns True on success, false on failure.
  bool open_project(const QString& path) override {

    game_dir = QDir(path);
    if (!game_dir.exists()) {
      return false;
    }

    auto model_path = game_dir.filePath("model.json");

    QFileInfo model_info(model_path);

    if (model_info.exists() && !project_model->open(model_path)) {
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
  /// Saves all project data.
  /// @returns True on success, false on failure.
  bool save_all() override {
    return project_model->save(game_dir.filePath("model.json"));
  }
};

} // namespace

ScopedPtr<ProjectManager> ProjectManager::make() {
  return new ProjectManagerImpl();
}

} // namespace tk

} // namespace herald
