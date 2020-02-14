#pragma once

#include "Room.h"

namespace herald {

template <typename T>
class ScopedPtr;

/// The default room model.
class DefaultRoom : public Room {
public:
  /// Creates a new default room instance.
  /// @returns A new default room instance.
  static ScopedPtr<DefaultRoom> make();
  /// Just a stub.
  virtual ~DefaultRoom() {}
};

} // namespace herald
