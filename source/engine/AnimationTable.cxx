#include <herald/AnimationTable.h>

#include <herald/Animation.h>
#include <herald/Index.h>
#include <herald/ScopedPtr.h>

#include <vector>

namespace herald {

namespace {

/// Implements the animation table interface.
class AnimationTableImpl final : public AnimationTable {
  /// The animation container.
  std::vector<ScopedPtr<Animation>> animations;
public:
  /// Adds an animation to the table.
  /// @param animation The animation to add.
  void add(ScopedPtr<Animation>&& animation) override {
    animations.emplace_back(std::move(animation));
  }
  /// Adds a single frame animation.
  /// @param texture_index The index of the texture containing the frame.
  void add_still_frame(Index texture_index) override {
    animations.emplace_back(Animation::make_single_frame(texture_index));
  }
  /// Accesses an animation at a specified index.
  const Animation* at(Index index) const noexcept override {
    if (index >= animations.size()) {
      return Animation::get_null_animation();
    } else {
      return animations[index].get();
    }
  }
};

} // namespace

ScopedPtr<AnimationTable> AnimationTable::make() {
  return new AnimationTableImpl;
}

} // namespace herald
