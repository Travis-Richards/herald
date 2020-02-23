#include "ProcessQueue.h"

#include "Console.h"

#include <herald/ScopedPtr.h>

#include <QProcess>
#include <QPushButton>
#include <QTextStream>

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
  /// The buttons to enable on completion of the queue.
  std::vector<QPushButton*> completion_buttons;
  /// The number of processes that failed.
  int failure_count;
  /// The number of processes that crashed.
  int crash_count;
public:
  /// Constructs a new instance of the process queue.
  /// @param c A pointer to the console to log output to.
  ProcessQueueImpl(Console* c) : console(c), failure_count(0), crash_count(0) {}
  /// Adds a process to the queue.
  /// @returns A pointer to the process that was added.
  QProcess* add() override {

    auto process = make_process();

    auto process_ptr = process.get();

    process_vec.emplace_back(std::move(process));

    return process_ptr;
  }
  /// Adds a button to a list of buttons to
  /// enable when the queue completes.
  void enable_on_completion(QPushButton* button) override {
    completion_buttons.emplace_back(button);
  }
  /// Starts all processes in the build queue.
  void start_all() override {

    failure_count = 0;

    crash_count = 0;

    for (auto& process : process_vec) {
      process->start();
    }
  }
protected:
  /// Creates a new process to be added to the queue.
  /// This function also does the job of connecting
  /// the process to the queue and console.
  /// @returns The created process.
  ScopedPtr<QProcess> make_process() {

    auto process = ScopedPtr<ConsoleProcess>(new ConsoleProcess(console));

    QProcess* process_ptr = process.get();

    auto finish_functor = [this, process_ptr](int exit_code, QProcess::ExitStatus status) {
      handle_finished(process_ptr, exit_code, status);
    };

    QObject::connect(process.get(), static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), finish_functor);

    return process;
  }
  /// Handles a finished process.
  /// @param process_ptr A pointer to the process that completed.
  /// This is used to remove the process from the process queue.
  /// @param exit_code The exit code of the process.
  /// If this is non-zero, then the process queue is
  /// considered to have failed.
  /// @param status The exit status of the process.
  /// If the status indicates that there was a crash,
  /// then it is indicated in the console message.
  void handle_finished(QProcess* process_ptr, int exit_code, QProcess::ExitStatus status) {

    if ((status == QProcess::NormalExit) && (exit_code != 0)) {
      failure_count++;
    } else if (status == QProcess::CrashExit) {
      crash_count++;
    }

    remove(process_ptr);
  }
  /// Removes a process from the queue.
  /// @param process_ptr A pointer to the process to remove.
  void remove(QProcess* process_ptr) {

    for (std::size_t i = 0; i < process_vec.size(); i++) {
      if (process_vec[i].get() == process_ptr) {
        process_vec.erase(process_vec.begin() + i);
        break;
      }
    }

    if (process_vec.empty()) {

      enable_buttons();

      if (failure_count || crash_count) {
        inform_build_failure();
      } else {
        console->println("Build complete", Console::Tag::Success);
      }
    }
  }
  /// Enables buttons in the completion button list.
  void enable_buttons() {
    for (auto* button : completion_buttons) {
      button->setEnabled(true);
    }
  }
  /// Informs the user, through the console, that the build has failed.
  /// Indicates the numbed of failed jobs and the number of crashed jobs.
  void inform_build_failure() {

    QString message;

    QTextStream stream(&message);

    stream << "Build failed";

    if (failure_count > 0) {
      stream << " (failures: " << failure_count << ")";
    }

    if (crash_count > 0) {
      stream << " (crashes: " << crash_count << ")";
    }

    stream << endl;

    console->println(message, Console::Tag::Error);
  }
};

} // namespace

ScopedPtr<ProcessQueue> ProcessQueue::make(Console* console) {
  return new ProcessQueueImpl(console);
}

} // namespace tk

} // namespace herald
