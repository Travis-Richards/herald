#include "WorkQueue.h"

#include <herald/ScopedPtr.h>

#include <herald/protocol/Command.h>

#include "Interpreter.h"

#include <utility>
#include <vector>

namespace herald {

namespace {

/// A type definition for an
/// entry within the work queue.
using WorkItem = std::pair<ScopedPtr<protocol::Command>, ScopedPtr<Interpreter>>;

/// The implementation of the work queue interface.
class WorkQueueImpl final : public WorkQueue {
  /// The items in the work queue.
  std::vector<WorkItem> items;
  /// A "null" command instance.
  ScopedPtr<protocol::Command> null_command;
  /// A "null" interpreter instance.
  ScopedPtr<Interpreter> null_interpreter;
public:
  /// Constructs a new work queue implementation instance.
  WorkQueueImpl() : null_command(protocol::Command::make_null()),
                    null_interpreter(Interpreter::make_null(nullptr)) {

  }
  /// Adds an item to the work queue.
  /// @param command
  void add(ScopedPtr<protocol::Command>&& cmd, Interpreter* interpreter) override {
    return items.emplace_back(std::move(cmd), interpreter);
  }
  /// Indicates whether or not the queue is empty.
  bool empty() const noexcept override {
    return items.empty();
  }
  /// Gets the current command pointer.
  const protocol::Command& get_current_command() const noexcept override {
    if (items.empty()) {
      return *null_command;
    } else {
      return *items[0].first;
    }
  }
  /// Gets the current interpreter pointer.
  Interpreter& get_current_interpreter() noexcept override {
    if (items.empty()) {
      return *null_interpreter;
    } else if (!items[0].second) {
      return *null_interpreter;
    } else {
      return *items[0].second;
    }
  }
  /// Removes the current work item.
  void pop() override {
    items.erase(items.begin());
  }
};

} // namespace

ScopedPtr<WorkQueue> WorkQueue::make() {
  return new WorkQueueImpl;
}

} // namespace herald
