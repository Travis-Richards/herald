#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

class Action;
class Index;

/// Used to contain the actions
/// available for game play.
class ActionTable {
public:
  /// Creates a new action table.
  /// @returns A new action table.
  static ScopedPtr<ActionTable> make();
  /// Just a stub.
  virtual ~ActionTable() {}
  /// Adds an action to the table.
  /// @param action The action to add to the table.
  virtual void add(const Action& action) = 0;
  /// Accesses an action at a specific index.
  /// @param index The index of the action to get.
  /// @returns A pointer to the specified action.
  /// If the index is out of bounds, then a "null"
  /// action is returned instead.
  virtual const Action* at(Index index) const noexcept = 0;
  /// Remove all actions from the table.
  virtual void clear() = 0;
};

} // namespace herald
