#include "ProcessQueue.h"

#include "Console.h"

#include <herald/ScopedPtr.h>

#include <QProcess>

#include <vector>

namespace herald {

namespace tk {

namespace {

/// A process started in the console.
class ConsoleProcess final : public QProcess {
  /// The console to write output content to.
  Console* console;
  /// Used for buffering error content
  /// until a newline is found.
  QString err_buffer;
  /// Used for buffering output content
  /// until a newline is found.
  QString out_buffer;
public:
  /// Constructs a new instance of a console process.
  /// @param c The console to write the output
  /// and error content to.
  ConsoleProcess(Console* c) : console(c) {
    connect(this, &QProcess::readyReadStandardOutput, this, &ConsoleProcess::read_stdout);
    connect(this, &QProcess::readyReadStandardError,  this, &ConsoleProcess::read_stderr);
  }
protected slots:
  /// Reads the process' standard output.
  void read_stdout() {
    out_buffer += readAllStandardOutput();
    flush_stdout();
  }
  /// Reads the process' standard error output.
  void read_stderr() {
    err_buffer += readAllStandardError();
    flush_stderr();
  }
protected:
  /// Writes all completed lines to the console.
  void flush_stdout() {

    auto lines = remove_completed_lines(out_buffer);

    for (const auto& line : lines) {
      console->log_stdout(line + '\n');
    }
  }
  /// Writes all completed lines to the console.
  void flush_stderr() {

    auto lines = remove_completed_lines(err_buffer);

    for (const auto& line : lines) {
      console->log_stderr(line + '\n');
    }
  }
  /// Removes all completed lines from a buffer
  /// and puts them into a string list.
  static QStringList remove_completed_lines(QString& buffer) {

    QStringList output;

    QString line;

    int last_found_line = 0;

    for (int i = 0; i < buffer.size(); i++) {

      if ((buffer[i] == '\r') || (buffer[i] == '\n')) {

        if (!line.isEmpty()) {
          output << line;
          line.clear();
        }

        last_found_line = i;

      } else {

        line += buffer[i];
      }
    }

    buffer.remove(0, last_found_line);

    return output;
  }
};

/// The implemention of the process queue interface.
class ProcessQueueImpl final : public ProcessQueue {
  /// The console to log process output to.
  Console* console;
  /// The container of processes started by the queue.
  std::vector<ScopedPtr<QProcess>> process_vec;
public:
  /// Constructs a new instance of the process queue.
  /// @param c A pointer to the console to log output to.
  ProcessQueueImpl(Console* c) : console(c) {}
  /// Adds a process to the queue.
  /// @returns A pointer to the process that was added.
  QProcess* add() override {

    auto process = make_process();

    auto process_ptr = process.get();

    process_vec.emplace_back(std::move(process));

    return process_ptr;
  }
protected:
  /// Creates a new process to be added to the queue.
  /// This function also does the job of connecting
  /// the process to the queue and console.
  /// @returns The created process.
  ScopedPtr<QProcess> make_process() {

    auto process = ScopedPtr<ConsoleProcess>(new ConsoleProcess(console));

    return process;
  }
};

} // namespace

ScopedPtr<ProcessQueue> ProcessQueue::make(Console* console) {
  return new ProcessQueueImpl(console);
}

} // namespace tk

} // namespace herald
