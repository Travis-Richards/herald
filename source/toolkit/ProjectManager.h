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
class SourceTreeModel;

/// Used to manage the data of a project.
class ProjectManager {
public:
  /// Creates a new project manager.
  /// @returns A new project manager instance.
  static ScopedPtr<ProjectManager> make();
  /// Just a stub.
  virtual ~ProjectManager() {}
  /// Adds a new texture to the project.
  /// @param path The path of the texture to add.
  /// @returns The name used to identify the texture.
  virtual QString add_texture(const QString& path) = 0;
  /// Deletes a texture from the project.
  /// @param name The name of the texture to delete.
  virtual void delete_texture(const QString& name) = 0;
  /// Accesses a pointer to the language used for the project.
  virtual Language* get_language() noexcept = 0;
  /// Accesses the source tree model.
  /// @returns A pointer to the source tree model.
  virtual SourceTreeModel* get_source_tree_model() noexcept = 0;
  /// Lists the textures in the project.
  /// @returns A list of texture names that exist in the project.
  virtual QStringList list_textures() const = 0;
  /// Indicates whether or not there are unsaved changes in the project.
  /// @returns True if there are unsaved changes, false if everything is saved.
  virtual bool has_unsaved_changes() const = 0;
  /// Opens a project.
  /// @param game_path The path to the game project.
  /// @returns True on success, false on failure.
  virtual bool open(const QString& path) = 0;
  /// Opens the game information for the project.
  /// @returns The game info for the project.
  virtual ScopedPtr<GameInfo> open_info() const = 0;
  /// Renames a texture.
  /// @param index The index of the texture to rename.
  /// @param name The name to give the texture.
  virtual void rename_texture(std::size_t index, const QString& name) = 0;
  /// Saves the project data.
  /// @returns True on success, false on failure.
  virtual bool save_all() = 0;
  /// Gets the path of a texture.
  /// @param name The name of the texture to get the path of.
  /// @returns The path of the specified texture.
  virtual QString texture_path(const QString& name) = 0;
};

} // namespace tk

} // namespace herald
