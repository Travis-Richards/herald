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
  /// Accesses a pointer to the Qt item model for the source tree.
  virtual QAbstractItemModel* get_model() noexcept = 0;
  /// Accesses the index that points to the root of the source tree.
  virtual QModelIndex get_root() const = 0;
  /// Opens a source file from a model index.
  /// @param index The index of the file from the source tree model.
  /// @returns The file corresponding to the specified index.
  virtual ScopedPtr<SourceFile> open(const QModelIndex& index) const = 0;
};

} // namespace tk

} // namespace herald
