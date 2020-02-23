#pragma once

class QWidget;
class QString;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// The console is where program
/// errors get logged to.
class Console {
public:
  /// Creates a new console instance.
  /// @param parent A pointer to the parent widget.
  /// @returns A new console instance.
  static ScopedPtr<Console> make(QWidget* parent);
  /// Just a stub.
  virtual ~Console() {}
  /// Clears the contents on the console.
  virtual void clear() = 0;
  /// Accesses the console widget.
  /// This is required when adding the console to a layout.
  /// @returns A pointer to the console widget.
  virtual QWidget* get_widget() noexcept = 0;
  /// Logs information sent from a process' standard output file.
  virtual void log_stdout(const QString& str) = 0;
  /// Logs information sent from a process' standard error file.
  virtual void log_stderr(const QString& str) = 0;
  /// Prints a message to the console, with a newline
  /// automatically added to the end of the string.
  /// @param msg The message to log.
  virtual void println(const QString& msg) = 0;
};

} // namespace tk

} // namespace herald
