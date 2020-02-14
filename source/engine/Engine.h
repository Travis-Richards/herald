#pragma once

#include <cstddef>

namespace herald {

class Model;

/// The base class for a rendering pipeline.
class Engine {
public:
  /// Just a stub.
  virtual ~Engine() {}
  /// Accesses the game model contained by the engine implementation.
  /// @returns A pointer to the game model.
  virtual Model* get_model() = 0;
  /// Advances the game forward by a certain amount of time.
  /// This updates all animations and movements that are active.
  /// @param delta The amount of time to move forward by, in terms of milliseconds.
  virtual void advance(std::size_t delta_ms) = 0;
};

} // namespace herald
