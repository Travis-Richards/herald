#pragma once

class QString;
class QTextDocument;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// Enumerates the several types of source files.
enum class SourceFileType : int {
  /// An invalid source file. This happens
  /// when an item is opened that cannot have
  /// representation as a source code file.
  Invalid,
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
  /// Accesses the text document containing the code.
  /// @returns A pointer to the text document containing the code.
  virtual QTextDocument* get_code() noexcept = 0;
  /// Accesses the text document containing the code.
  /// @returns A pointer to the text document containing the code.
  virtual const QTextDocument* get_code() const noexcept = 0;
  /// Accesses the type of the source code file.
  virtual SourceFileType get_type() const noexcept = 0;
};

} // namespace tk

} // namespace herald
