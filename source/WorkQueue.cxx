#include "WorkQueue.h"

#include "Interpreter.h"
#include "ScopedPtr.h"

#include "lang/Command.h"

#include <utility>
#include <vector>

namespace herald {

namespace {

/// A type definition for an
/// entry within the work queue.
using WorkItem = std::pair<ScopedPtr<Command>, ScopedPtr<Interpreter>>;

/// The implementation of the work queue interface.
class WorkQueueImpl final : public WorkQueue {
  /// The items in the work queue.
  std::vector<WorkItem> items;
  /// A "null" command instance.
  ScopedPtr<Command> null_command;
  /// A "null" interpreter instance.
  ScopedPtr<Interpreter> null_interpreter;
public:
  /// Constructs a new work queue implementation instance.
  WorkQueueImpl() : null_command(Command::make_null()),
                    null_interpreter(Interpreter::make_null(nullptr)) {

  }
  /// Adds an item to the work queue.
  /// @param command
  void add(Command* command, Interpreter* interpreter) override {
    return items.emplace_back(command, interpreter);
  }
  /// Indicates whether or not the queue is empty.
  bool empty() const noexcept override {
    return items.empty();
  }
  /// Gets the current command pointer.
  const Command* get_current_command() const noexcept override {
    if (items.empty()) {
      return nullptr;
    } else {
      return items[0].first.get();
    }
  }
  /// Gets the current interpreter pointer.
  Interpreter* get_current_interpreter() noexcept override {
    if (items.empty()) {
      return nullptr;
    } else {
      return items[0].second.get();
    }
  }
  /// Removes the current work item.
  void pop() override {
  }
};

} // namespace

ScopedPtr<WorkQueue> WorkQueue::make() {
  return new WorkQueueImpl;
}

} // namespace herald
