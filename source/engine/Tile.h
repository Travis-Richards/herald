#pragma once

namespace herald {

class Index;

/// Represents a single, rectangular
/// tile used to make up a room's appearance.
class Tile {
public:
  /// Accesses a reference to a null tile.
  /// @returns A reference to a null tile.
  static Tile* get_null_tile() noexcept;
  /// Just a stub.
  virtual ~Tile() {}
  /// Queues an animation change.
  /// @param animation The animation to switch to.
  virtual void queue_animation(Index animation) = 0;
};

} // namespace herald
