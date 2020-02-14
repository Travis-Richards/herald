#pragma once

#include "Background.h"

namespace herald {

template <typename T>
class ScopedPtr;

/// The default implementation of the background animation.
class DefaultBackground : public Background {
public:
  /// Creates a new instance of the default background.
  /// @returns A pointer to a new default background instance.
  static ScopedPtr<DefaultBackground> make();
  /// Just a stub.
  virtual ~DefaultBackground() {}
};

} // namespace herald
