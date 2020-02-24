#pragma once

#include <cstddef>

class QString;
class QStringList;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// The texture table is the table
/// of images being used by the project.
/// Textures are indexed by name and
/// loaded by path.
class TextureTable {
public:
  /// Just a stub.
  virtual ~TextureTable() { }
  /// Makes a new entry for a texture
  /// in the texture table.
  /// @param path The path of the texture to add to the table.
  /// @returns The ID of the texture.
  virtual QString add(const QString& path) = 0;
  /// Gets the path of a texture.
  /// @param name The name of the texture to get the path of.
  /// @returns The path of the specified texture.
  /// If the texture doesn't exist, then an empty string is returned.
  virtual QString get_path(const QString& name) const = 0;
  /// Removes a texture from the texture table.
  /// @param name The name of the texture to remove.
  virtual void remove(const QString& name) = 0;
  /// Renames a texture.
  /// @param index The index of the texture to rename.
  /// @param name The new name to give the texture.
  /// @returns True on success, false on failure.
  virtual bool rename(std::size_t index, const QString& name) = 0;
  /// Lists all the textures in the table.
  /// @returns A list of all textures in the table.
  virtual QStringList list() const = 0;
};

/// A project's data model.
class ProjectModel {
public:
  /// Creates a new project model instance.
  /// @returns A pointer to a new project model.
  static ScopedPtr<ProjectModel> make();
  /// Just a stub.
  virtual ~ProjectModel() {}
  /// Accesses the model's texture table.
  /// @returns A pointer to the model's texture table.
  virtual TextureTable* get_texture_table()  = 0;
  /// Opens a project model.
  /// @param path The path to the model to open.
  /// It's expected that this is a JSON file.
  /// @returns True on success, false on failure.
  virtual bool open(const QString& path) = 0;
};

} // namespace tk

} // namespace herald
