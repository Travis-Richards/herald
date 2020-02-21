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
  /// The code associated with the file.
  QString code;
public:
  /// Opens a source file.
  /// @param path The path to the source file to open.
  DefaultSourceFile(const QString& path) : type(SourceFileType::Invalid) {
    open(path);
  }
  /// Accesses the IO device to read and write data.
  const QString& get_code() const noexcept override {
    return code;
  }
  /// Accesses the type associated with the source file.
  SourceFileType get_type() const noexcept override {
    return type;
  }
protected:
  /// Opens a file specified by the path.
  /// @param path The path of the file to open.
  /// @returns True on success, false on failure.
  bool open(const QString& path) {

    QFileInfo file_info(path);

    if (!file_info.exists() || !file_info.isFile()) {
      return false;
    }

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return false;
    }

    code = file.readAll();
    type = parse_type(file_info.suffix());

    return true;
  }
  /// Parses the file extension for the source file type.
  /// @param ext The extension of the source file.
  /// @returns The type of the source file.
  /// If it's not recognized, then @ref SourceFileType::Invalid is returned.
  static SourceFileType parse_type(const QString& ext) {
    if (ext == "java") {
      return SourceFileType::Java;
    } else if (ext == "py") {
      return SourceFileType::Python;
    } else {
      return SourceFileType::Invalid;
    }
  }
};

} // namespace

ScopedPtr<SourceFile> SourceFile::from_fs(const QString& path) {
  return new DefaultSourceFile(path);
}

} // namespace tk

} // namespace herald
