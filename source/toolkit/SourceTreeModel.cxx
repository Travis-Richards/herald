#include "SourceTreeModel.h"

#include "SourceFile.h"

#include <herald/ScopedPtr.h>

#include <QFileSystemModel>
#include <QSaveFile>
#include <QTextDocument>

#include <map>

#include <climits>

namespace herald {

namespace tk {

namespace {

/// The full implementation of the source tree model interface.
class SourceTreeModelImpl final : public SourceTreeModel {
  /// A map of files currently opened.
  std::map<QString, ScopedPtr<SourceFile>> opened_files;
  /// The default file extension for file creation.
  QString default_extension;
public:
  /// Constructs a new instance of the source manager.
  /// @param path The path to the source tree manager.
  /// @param parent A pointer to the parent object.
  SourceTreeModelImpl(const QString& path, QObject* parent) : SourceTreeModel(path, parent) { }
  /// Creates a new, unique source file.
  /// @param parent The index of the folder to put the file into.
  /// @returns true on success, false on failure.
  bool create_source_file(const QModelIndex& parent) override {

    auto prefix = filePath(parent);

    for (int i = 0; i < INT_MAX; i++) {

      auto path = make_source_file_path(prefix, i);

      QFileInfo file_info(path);
      if (!file_info.exists()) {
        return QFile(path).open(QIODevice::WriteOnly);
      }
    }

    return true;
  }
  /// Creates a unique sub directory.
  /// @param parent The index to the parent directory.
  /// @returns True on success, false on failure.
  bool create_dir(const QModelIndex& parent) override {

    auto prefix = filePath(parent);

    for (int i = 0; i < INT_MAX; i++) {

      auto name = make_dir_name(i);
      QDir dir(prefix);
      if (dir.mkdir(name)) {
        return true;
      }
    }

    return false;
  }
  /// Opens a file at a specific index.
  /// @param index The index of the file to open.
  /// @returns A pointer to the specified source file.
  SourceFile* open(const QModelIndex& index) override {

    auto path = filePath(index);

    auto it = opened_files.find(path);
    if (it == opened_files.end()) {
      return open_without_cache(path);
    } else {
      return it->second.get();
    }
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
  /// Attempts at making a unique source file path.
  /// @param prefix The prefix to prepend to the created path.
  /// @param index The index of the iteration of which a unique file was attempted to be made.
  /// @returns A unique file path.
  QString make_source_file_path(const QString& prefix, int index) {

    auto path = prefix + QDir::separator();

    if (index == 0) {
      path += "Untitled";
    } else {
      path += "Untitled " + QString::number(index);
    }

    return path + default_extension;
  }
  /// Creates a new directory name.
  /// @param index The number to append to the name.
  /// If this value is zero, no number is appended.
  QString make_dir_name(int index) {
    QString name("New Folder");
    if (index != 0) {
      return name + " " + QString::number(index);
    } else {
      return name;
    }
  }
};

} // namespace

ScopedPtr<SourceTreeModel> SourceTreeModel::make(const QString& path, QObject* parent) {
  return new SourceTreeModelImpl(path, parent);
}

} // namespace tk

} // namespace herald
