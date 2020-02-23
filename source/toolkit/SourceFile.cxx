#include "SourceFile.h"

#include <herald/ScopedPtr.h>

#include <QFile>
#include <QFileInfo>
#include <QString>

namespace herald {

namespace tk {

namespace {

/// The default source file implementation.
/// The source file is taken from the file system.
class DefaultSourceFile final : public SourceFile {
  /// The type associated with this source file.
  SourceFileType type;
  /// The filename of the source file.
  QString name;
  /// The full path to the source file.
  QString path;
public:
  /// Opens a source file.
  /// @param filename The path to the source file to open.
  DefaultSourceFile(const QString& filename) : type(SourceFileType::Unknown) {
    QFileInfo file_info(filename);
    path = file_info.absoluteFilePath();
    name = file_info.fileName();
    type = parse_type(file_info.suffix());
  }
  /// Accesses the full path of the source file.
  const QString& get_id() const noexcept override {
    return path;
  }
  /// Accesses the filename of the source file.
  const QString& get_name() const noexcept override {
    return name;
  }
  /// Accesses the type associated with the source file.
  SourceFileType get_type() const noexcept override {
    return type;
  }
  /// Reads all of the source file's content.
  /// @returns The content of the source file.
  QString read_content() const override {

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return QString();
    }

    return file.readAll();
  }
protected:
  /// Parses the file extension for the source file type.
  /// @param ext The extension of the source file.
  /// @returns The type of the source file.
  /// If it's not recognized, then @ref SourceFileType::Invalid is returned.
  static SourceFileType parse_type(const QString& ext) {
    if (ext == "java") {
      return SourceFileType::Java;
    } else if (ext == "py") {
      return SourceFileType::Python;
    } else if ((ext == "pyc")
            || (ext == "class")
            || (ext == "bin")) {
      return SourceFileType::Binary;
    } else {
      return SourceFileType::Unknown;
    }
  }
};

} // namespace

ScopedPtr<SourceFile> SourceFile::from_fs(const QString& path) {
  return new DefaultSourceFile(path);
}

} // namespace tk

} // namespace herald
