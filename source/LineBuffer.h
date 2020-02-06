#pragma once

#include <QObject>

class QProcess;
class QString;

/// This line buffer is primary used for
/// accumulating lines echoed from a running
/// process.
class LineBuffer : public QObject {
  Q_OBJECT
public:
  /// Creates a new line buffer for a process' standard output file.
  /// @param process The process to accumulate lines for.
  /// @returns A new line buffer instance.
  static LineBuffer* from_process_stdout(QProcess& process, QObject* parent);
  /// Creates a new line buffer for a process' standard error file.
  /// @param process The process to read the standard error from.
  /// @returns A new line buffer instance.
  static LineBuffer* from_process_stderr(QProcess& process, QObject* parent);
  /// Just a stub.
  virtual ~LineBuffer() {}
signals:
  /// This signal is emitted when a complete
  /// line is read from the IO device.
  void line(const QString& line);
protected:
  /// Constructs a new line buffer instance.
  /// @param parent A pointer to the parent object.
  LineBuffer(QObject* parent = nullptr);
};
