#pragma once

#include <cstddef>

class QAbstractItemModel;
class QModelIndex;
class QString;
class QStringList;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class GameInfo;
class Language;
class Project;
class SourceTreeModel;

/// Used to manage the data of a project.
class ProjectManager {
public:
  /// Creates a new project manager.
  /// @returns A new project manager instance.
  static ScopedPtr<ProjectManager> make();
  /// Just a stub.
  virtual ~ProjectManager() {}
  /// Accesses a pointer to the language used for the project.
  virtual Language* get_language() noexcept = 0;
  /// Accesses a pointer to the project data.
  virtual Project* get_project() noexcept = 0;
  /// Accesses the source tree model.
  /// @returns A pointer to the source tree model.
  virtual SourceTreeModel* get_source_tree_model() noexcept = 0;
  /// Indicates whether or not there are unsaved changes in the project.
  /// @returns True if there are unsaved changes, false if everything is saved.
  virtual bool has_unsaved_changes() const = 0;
  /// Opens the game information for the project.
  /// @returns The game info for the project.
  virtual ScopedPtr<GameInfo> open_info() const = 0;
  /// Opens a project.
  /// @param path The path to the project directory.
  /// @returns True on success, false on failure.
  virtual bool open_project(const QString& path) = 0;
  /// Saves the project data.
  /// @returns True on success, false on failure.
  virtual bool save_all() = 0;
};

} // namespace tk

} // namespace herald
