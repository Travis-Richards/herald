#pragma once

#include <QFileSystemModel>

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class SourceFile;

/// The data model of a project's source code tree.
class SourceTreeModel : public QFileSystemModel {
public:
  /// Creates a new source manager instance for a specified directory.
  /// @param path The path of the directory containing the source files to manage.
  /// @param parent An optional pointer to the parent object.
  /// @returns A new source manager instance.
  static ScopedPtr<SourceTreeModel> make(const QString& path, QObject* parent = nullptr);
  /// Constructs the base of the source tree model.
  /// @param path The path of the source tree.
  /// @param parent An optional pointer to the parent object.
  SourceTreeModel(const QString& path, QObject* parent) : QFileSystemModel(parent) {
    setRootPath(path);
    setReadOnly(false);
  }
  /// Just a stub.
  virtual ~SourceTreeModel() {}
  /// Creates a new, unique directory.
  /// @param parent The index of the parent folder to make the directory in.
  /// @returns True on success, false on failure.
  virtual bool create_dir(const QModelIndex& parent) = 0;
  /// Creates a new, unique source file.
  /// @param parent The index of the parent folder to make the file in.
  /// @returns True on success, false on failure.
  virtual bool create_source_file(const QModelIndex& parent) = 0;
  /// Opens a source file from a model index.
  /// @param index The index of the file from the source tree model.
  /// @returns The file corresponding to the specified index.
  /// The source manager retains ownership of the source file.
  virtual SourceFile* open(const QModelIndex& index) = 0;
};

} // namespace tk

} // namespace herald
