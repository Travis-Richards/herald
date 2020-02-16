#pragma once

class QString;
class QStringList;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

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
  /// Lists the textures in the project.
  /// @returns A list of texture names that exist in the project.
  virtual QStringList list_textures() const = 0;
  /// Opens a project.
  /// @param game_path The path to the game project.
  /// @returns True on success, false on failure.
  virtual bool open(const QString& path) = 0;
};

} // namespace tk

} // namespace herald
