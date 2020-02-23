#pragma once

class QString;
class QTextDocument;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// Enumerates the several types of source files.
enum class SourceFileType : int {
  /// Indicates that the source file
  /// type is unknown.
  Unknown,
  /// Indicates that this is a binary
  /// file type and should not put into
  /// a code editor.
  Binary,
  /// A Java source code file.
  Java,
  /// A Python source code file.
  Python
};

/// A source code file.
/// This class was made in case the source
/// files are to be retrieved from somewhere
/// other than the file system.
class SourceFile {
public:
  /// Opens a source file from a specified file system path.
  /// @param path The path to open the file from.
  /// @returns A pointer to the file instance.
  static ScopedPtr<SourceFile> from_fs(const QString& path);
  /// Just a stub.
  virtual ~SourceFile() {}
  /// Accesses an ID of the source file.
  /// On a file system, this would be the full path of the source file.
  virtual const QString& get_id() const noexcept = 0;
  /// Accesses the name given to the source file.
  /// This includes the file extension.
  virtual const QString& get_name() const noexcept = 0;
  /// Accesses the type of the source code file.
  virtual SourceFileType get_type() const noexcept = 0;
  /// Reads the contents of the source file.
  /// @returns The contents of the source file.
  virtual QString read_content() const = 0;
};

} // namespace tk

} // namespace herald
