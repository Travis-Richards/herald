#pragma once

class QAbstractItemModel;
class QModelIndex;
class QString;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class SourceFile;

/// Used for managing the source
/// tree of a project.
class SourceManager {
public:
  /// Creates a new source manager instance for a specified directory.
  /// @param path The path of the directory containing the source files to manage.
  /// @returns A new source manager instance.
  static ScopedPtr<SourceManager> make(const QString& path);
  /// Just a stub.
  virtual ~SourceManager() {}
  /// Creates a new, unique, source file.
  /// @returns True on success, false on failure.
  virtual bool create_source_file() = 0;
  /// Accesses a pointer to the Qt item model for the source tree.
  virtual QAbstractItemModel* get_model() noexcept = 0;
  /// Accesses the index that points to the root of the source tree.
  virtual QModelIndex get_root() const = 0;
  /// Indicates if there are unsaved changes.
  /// @returns True if there are unsaved changes, false otherwise.
  virtual bool has_unsaved_changes() const = 0;
  /// Opens a source file from a model index.
  /// @param index The index of the file from the source tree model.
  /// @returns The file corresponding to the specified index.
  /// The source manager retains ownership of the source file.
  virtual SourceFile* open(const QModelIndex& index) = 0;
  /// Save all files that have been opened and modified.
  /// @returns True if all files saved succesfully, false if
  /// there was at least one error.
  virtual bool save_modified() = 0;
};

} // namespace tk

} // namespace herald
