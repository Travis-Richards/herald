#include "LineBuffer.h"

#include <QByteArray>
#include <QProcess>
#include <QString>

namespace {

/// A line buffer for a process.
class ProcessLineBuffer final : public LineBuffer {
  /// The process being read from.
  QProcess& process;
  /// The line data being accumulated at each read operation.
  QByteArray line_buffer;
public:
  /// Constructs a line buffer for a process.
  /// @param p The process to read from.
  /// @param parent A pointer to the parent object.
  /// @param read_err Whether or not to read from the standard error output.
  ProcessLineBuffer(QProcess& p, QObject* parent, bool read_err = false)
    : LineBuffer(parent), process(p) {

    if (read_err) {
      connect(&p, &QProcess::readyReadStandardError, this, &ProcessLineBuffer::read_err);
    } else {
      connect(&p, &QProcess::readyReadStandardOutput, this, &ProcessLineBuffer::read_out);
    }
  }
protected slots:
  /// Reads data from the process standard output.
  void read_out() {
    return handle(process.readAllStandardOutput());
  }
  /// Reads data from the process standard error output.
  void read_err() {
    return handle(process.readAllStandardError());
  }
  /// Handles data from the process.
  /// If a line feed character is found, then a line
  /// is emitted as a signal and a newline is started.
  void handle(const QByteArray& added_data) {
    for (auto byte : added_data) {
      line_buffer.append(byte);
      if (byte == '\n') {
        emit line(QString(line_buffer));
        line_buffer.clear();
      }
    }
  }
};

} // namespace

LineBuffer* LineBuffer::from_process_stdout(QProcess& process, QObject* parent) {
  return new ProcessLineBuffer(process, parent);
}

LineBuffer* LineBuffer::from_process_stderr(QProcess& process, QObject* parent) {
  return new ProcessLineBuffer(process, parent, true /* read stderr */);
}

LineBuffer::LineBuffer(QObject* parent) : QObject(parent) {

}
