#pragma once

#include "Model.h"

namespace herald {

template <typename T>
class ScopedPtr;

/// This is the data model implemented
/// for the default rendering engine.
class DefaultModel : public Model {
public:
  /// Creates a new instance of the default model.
  /// @returns A new default model instance.
  static ScopedPtr<DefaultModel> make();
  /// Just a stub.
  virtual ~DefaultModel() {}
};

} // namespace herald
