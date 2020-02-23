#include "SourceManager.h"

#include "SourceFile.h"

#include <herald/ScopedPtr.h>

#include <QFileSystemModel>
#include <QSaveFile>
#include <QTextDocument>

#include <map>

namespace herald {

namespace tk {

namespace {

/// The default implementation of the source tree manager.
/// Uses a file system model in the underlying implementation.
class DefaultSourceManager final : public SourceManager {
  /// The file system model for the source tree.
  QFileSystemModel model;
  /// A map of files currently opened.
  std::map<QString, ScopedPtr<SourceFile>> opened_files;
public:
  /// Constructs a new instance of the source manager.
  /// @param path The path to the source tree manager.
  DefaultSourceManager(const QString& path) {
    model.setRootPath(path);
  }
  /// Creates a new, unique source file.
  /// @param ext The extension to give the source file.
  /// @returns True on success, false on failure.
  bool create_source_file(const QString& ext) override {

    for (int i = 0; i < 256; i++) {

      auto path = make_source_file_path(i, ext);

      QFileInfo file_info(path);
      if (!file_info.exists()) {
        return QFile(path).open(QIODevice::WriteOnly);
      }
    }

    return true;
  }
  /// Accesses a pointer to the model.
  QAbstractItemModel* get_model() noexcept override {
    return &model;
  }
  /// Accesses the index that points to the root of the source tree.
  QModelIndex get_root() const override {
    return model.index(model.rootPath());
  }
  /// Indicates if any of the source code files
  /// has unsaved changes.
  bool has_unsaved_changes() const override {
    return false;
  }
  /// Opens a file at a specific index.
  /// @param index The index of the file to open.
  /// @returns A pointer to the specified source file.
  SourceFile* open(const QModelIndex& index) override {

    auto path = model.filePath(index);

    auto it = opened_files.find(path);
    if (it == opened_files.end()) {
      return open_without_cache(path);
    } else {
      return it->second.get();
    }
  }
  /// Gets the path of a specified file.
  /// @param index The index of the file to get the path of.
  /// @returns The path of the specified file.
  QString path_of(const QModelIndex& index) override {
    return model.filePath(index);
  }
  /// Saves all modified files.
  /// @returns True on success, false on failure.
  bool save_modified() override {
    return true;
  }
protected:
  /// Opens a file without checking to see if it's in cache.
  /// @param path The path of the file to open.
  /// @returns a pointer to the specified file.
  SourceFile* open_without_cache(const QString& path) {

    auto source_file = SourceFile::from_fs(path);

    auto* source_file_ptr = source_file.get();

    opened_files[path] = std::move(source_file);

    return source_file_ptr;
  }
  /// Creates a a source file path.
  /// @param index The number assigned to the source file path.
  /// If this is equal to zero, then no number is appended to the source file path.
  /// @param ext The extension to give the source file.
  /// @returns A unique source file path.
  /// This path is used when creating new source files
  /// using a generated file name.
  QString make_source_file_path(int index, const QString& ext) {

    auto path = QDir::cleanPath(model.rootPath() + QDir::separator() + "Untitled " + QString::number(index));

    path += ext;

    return path;
  }
};

} // namespace

ScopedPtr<SourceManager> SourceManager::make(const QString& path) {
  return new DefaultSourceManager(path);
}

} // namespace tk

} // namespace herald
