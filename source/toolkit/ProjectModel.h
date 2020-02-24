#pragma once

#include <cstddef>

class QByteArray;
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
  /// Gets the image data of a texture.
  /// @param name The name of the texture to get the image data of.
  /// @returns The image data of the specified image.
  /// If the texture isn't found, then an empty byte array is returned.
  virtual QByteArray get_data(const QString& name) const = 0;
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
  /// Whether or not the model has been modified.
  bool modified;
public:
  /// Creates a new project model instance.
  /// @returns A pointer to a new project model.
  static ScopedPtr<ProjectModel> make();
  /// Constructs the base project model.
  constexpr ProjectModel() noexcept : modified(false) {}
  /// Just a stub.
  virtual ~ProjectModel() {}
  /// Gets a non-const pointer to the texture table.
  /// @returns A pointer to the model's texture table.
  virtual TextureTable* modify_texture_table() = 0;
  /// Accesses the texture table for reading only
  /// This prevents the modification flag fron being set.
  /// @returns A constant pointer to the texture table.
  virtual const TextureTable* access_texture_table() const = 0;
  /// Indicates whether or not the project has been modified.
  /// @returns True if the project has been modified, false otherwise.
  inline bool is_modified() const noexcept { return modified; }
  /// Opens a project model.
  /// @param path The path to the model to open.
  /// It's expected that this is a JSON file.
  /// @returns True on success, false on failure.
  virtual bool open(const QString& path) = 0;
  /// Saves the project data.
  /// The data is first saved to a swap file
  /// before replacing the current file.
  /// The modification flag is set to false
  /// after calling this function.
  /// @param path The path to save the data to.
  /// @returns True on success, false on failure.
  virtual bool save(const QString& path) = 0;
protected:
  /// Sets the model's modification flag.
  /// @param value The value to set the modification flag to.
  inline void set_modified_flag(bool value) noexcept {
    modified = value;
  }
};

} // namespace tk

} // namespace herald
