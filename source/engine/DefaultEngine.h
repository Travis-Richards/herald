#pragma once

#include "Engine.h"

namespace herald {

template <typename T>
class ScopedPtr;

class Target;

/// The default implementation of the rendering engine.
class DefaultEngine : public Engine {
public:
  /// Creates a new default engine for a virtual target.
  /// @param target The target to render the audio and visual data to.
  /// @returns A new default engine instance.
  static ScopedPtr<DefaultEngine> make(Target& target);
  /// Just a stub.
  virtual ~DefaultEngine() {}
};

} // namespace herald
