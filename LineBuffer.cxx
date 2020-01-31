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
  ProcessLineBuffer(QProcess& p, QObject* parent)
    : LineBuffer(parent), process(p) {

    connect(&p, &QProcess::readyReadStandardOutput, this, &ProcessLineBuffer::read);
  }
protected slots:
  /// Reads data from the process standard output.
  /// If a line feed character is found, then a line
  /// is emitted as a signal and a newline is started.
  void read() {
    auto added_data = process.readAllStandardOutput();
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

LineBuffer::LineBuffer(QObject* parent) : QObject(parent) {

}
