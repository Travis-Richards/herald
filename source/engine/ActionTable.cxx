#include <herald/ActionTable.h>

#include <herald/Action.h>
#include <herald/ScopedPtr.h>

#include <vector>

namespace herald {

namespace {

/// The implementation of the action table interface.
class ActionTableImpl final : public ActionTable {
  /// The actions part of the table.
  std::vector<Action> actions;
  /// The action returned on out of bounds access.
  Action null_action;
public:
  /// Adds an action to the table.
  /// @param action The action to add.
  void add(const Action& action) override {
    actions.emplace_back(action);
  }
  /// Accesses an action at a specific index.
  const Action* at(Index index) const noexcept override {
    if (index >= actions.size()) {
      return &null_action;
    } else {
      return &actions[index];
    }
  }
  /// Clears all actions from the table.
  void clear() override {
    actions.clear();
  }
};

} // namespace

ScopedPtr<ActionTable> ActionTable::make() {
  return new ActionTableImpl();
}

} // namespace herald
