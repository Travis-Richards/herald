#pragma once

class QProcess;
class QPushButton;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class Console;

/// A queue for starting processes.
class ProcessQueue {
public:
  /// Creates a new process queue.
  /// @param console A pointer to the console
  /// that the process output will be logged to.
  /// @returns A pointer to a new process queue.
  static ScopedPtr<ProcessQueue> make(Console* console);
  /// Just a stub.
  virtual ~ProcessQueue() {}
  /// Adds a process to the process queue.
  /// @returns A pointer to the newly made process.
  /// This may be used to set the process parameters
  /// before launching the process.
  virtual QProcess* add() = 0;
  /// Starts all processes in the queue.
  virtual void start_all() = 0;
};

} // namespace tk

} // namespace herald
