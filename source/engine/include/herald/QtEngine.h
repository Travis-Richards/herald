#pragma once

#include "Engine.h"

namespace herald {

template <typename T>
class ScopedPtr;

class QtTarget;

/// Implements the engine interface using Qt.
class QtEngine : public Engine {
public:
  /// Creates a new instance of the Qt engine.
  /// @param target The target to render to.
  /// @returns A new Qt engine instance.
  static ScopedPtr<QtEngine> make(QtTarget* target);
  /// Just a stub.
  virtual ~QtEngine() {}
};

} // namespace herald
