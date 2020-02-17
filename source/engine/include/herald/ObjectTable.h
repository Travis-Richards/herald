#pragma once

#include <cstddef>

namespace herald {

class ActionTable;
class Index;
class Object;

/// Used to track items in the foreground,
/// such as players, enemies, or moveable items.
class ObjectTable {
public:
  /// Just a stub.
  virtual ~ObjectTable() {}
  /// Accesses an object at a specific index.
  /// @param index The index of the object to access.
  /// @returns A pointer to the specified object.
  /// If the index is out of bounds, then a null
  /// object instance is returned instead.
  virtual Object* at(Index index) = 0;
  /// Resizes the object table.
  /// @param count The new number of objects
  /// to have into the table.
  virtual void resize(std::size_t count) = 0;
  /// Indicates the number of objects in the object table.
  /// @returns The number of objects in the table.
  virtual std::size_t size() const noexcept = 0;
  /// Updates the animation indices from the action table.
  /// @param actions The action table to get the animation indices from.
  virtual void update_animation_indices(const ActionTable& actions);
};

} // namespace herald
