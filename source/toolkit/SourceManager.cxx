#include "SourceManager.h"

#include "SourceFile.h"

#include <herald/ScopedPtr.h>

#include <QFileSystemModel>

namespace herald {

namespace tk {

namespace {

/// The default implementation of the source tree manager.
/// Uses a file system model in the underlying implementation.
class DefaultSourceManager final : public SourceManager {
  /// The file system model for the source tree.
  QFileSystemModel model;
public:
  /// Constructs a new instance of the source manager.
  /// @param path The path to the source tree manager.
  DefaultSourceManager(const QString& path) {
    model.setRootPath(path);
  }
  /// Accesses a pointer to the model.
  QAbstractItemModel* get_model() noexcept override {
    return &model;
  }
  /// Accesses the index that points to the root of the source tree.
  QModelIndex get_root() const override {
    return model.index(model.rootPath());
  }
  /// Opens a file at a specific index.
  /// @param index The index of the file to open.
  /// @returns A pointer to the specified source file.
  ScopedPtr<SourceFile> open(const QModelIndex& index) const override {
    return SourceFile::from_fs(model.filePath(index));
  }
};

} // namespace

ScopedPtr<SourceManager> SourceManager::make(const QString& path) {
  return new DefaultSourceManager(path);
}

} // namespace tk

} // namespace herald
