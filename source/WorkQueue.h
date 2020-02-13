#pragma once

class Interpreter;

namespace herald {

template <typename T>
class ScopedPtr;

class Command;

/// Used for queing work items
/// to be handled by the game and game engine.
class WorkQueue {
public:
  /// Creates a new work queue.
  static ScopedPtr<WorkQueue> make();
  /// Just a stub.
  virtual ~WorkQueue() {}
  /// Adds a command and an interpreter to the work queue.
  /// @param command The command to add.
  /// @param interpreter The interpreter instance to add.
  virtual void add(Command* command, Interpreter* interpreter) = 0;
  /// Indicates whether or not the work queue is empty.
  /// @returns True if the work queue is empty, false if it's not.
  virtual bool empty() const noexcept = 0;
  /// Gets the current command in the work queue.
  /// If the work queue is empty, then a command
  /// is returned which as a data size of zero.
  virtual const Command* get_current_command() const noexcept = 0;
  /// Gets the current interpreter in the work queue.
  /// If the work queue is empty, then an interpreter
  /// is returned that does nothing.
  virtual Interpreter* get_current_interpreter() noexcept = 0;
  /// Removes the current item from the work queue.
  virtual void pop() = 0;
};

} // namespace herald
