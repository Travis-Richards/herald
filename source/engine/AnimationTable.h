#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

class Animation;
class Index;

/// Contains the animations to be used in the game.
class AnimationTable {
public:
  /// Creates a new animation table.
  /// @returns A new animation table instance.
  static ScopedPtr<AnimationTable> make();
  /// Just a stub.
  virtual ~AnimationTable() {}
  /// Adds an animation to the animation table.
  /// @param animation The animation to add to the table.
  virtual void add(ScopedPtr<Animation>&& animation) = 0;
  /// Accesses an animation at a specified index.
  /// @param index The index of the animation to get.
  /// @returns A pointer to the specified animation.
  /// If the index it out of bounds, then a pointer
  /// to a null animation instance is returned instead.
  virtual const Animation* at(Index index) const noexcept = 0;
};

} // namespace herald
