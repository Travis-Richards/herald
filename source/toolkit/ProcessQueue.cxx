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
public:
  /// Constructs a new instance of a console process.
  /// @param c The console to write the output
  /// and error content to.
  ConsoleProcess(Console* c) : console(c) {
    connect(this, &QProcess::readyReadStandardOutput, this, &ConsoleProcess::read_output);
    connect(this, &QProcess::readyReadStandardError,  this, &ConsoleProcess::read_error);
  }
protected slots:
  /// Reads the process' standard output.
  void read_output() {

    setReadChannel(QProcess::StandardOutput);

    console->log_stdout(readLine());
  }
  /// Reads the process' standard error output.
  void read_error() {

    setReadChannel(QProcess::StandardError);

    console->log_stderr(readLine());
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
