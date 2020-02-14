#include "Animation.h"

#include "Index.h"
#include "ScopedPtr.h"

#include <vector>

namespace herald {

namespace {

/// A null animation implementation.
class NullAnimation final : public Animation {
public:
  /// Does nothing.
  void add_frame(Index, std::size_t) override {}
  /// Does nothing.
  /// @returns An invalid index.
  Index calculate_texture_index(std::size_t) const noexcept override {
    return Index();
  }
};

NullAnimation null_animation;

/// Represents a single entry within an animation.
struct Frame final {
  /// The index of the texture that displays during this frame.
  Index texture;
  /// The number of milliseconds the frame should last.
  std::size_t delay_ms;
  /// Constructs a new frame instance with default data.
  constexpr Frame() noexcept : delay_ms(SIZE_MAX) {}
  /// Constructs a new frame with initialized data.
  /// @param t The index of the texture to display.
  /// @param d The delay, in milliseconds, of the frame.
  constexpr Frame(Index t, std::size_t d) : texture(t), delay_ms(d) {}
};

/// The implementation of the animation interface.
class AnimationImpl final : public Animation {
  /// The animation frame describe how
  /// the animation plays throughout its period of time.
  std::vector<Frame> frames;
  /// The total duration of the animation.
  std::size_t duration_ms;
public:
  /// Constructs a new instance of the animation implementation.
  AnimationImpl() : duration_ms(0) {}
  /// Adds a frame to the animation.
  /// @param texture The index of the texture to display for the frame.
  /// @param delay_ms The number of milliseconds the frame should last.
  void add_frame(Index texture, std::size_t delay_ms) override {
    frames.emplace_back(texture, delay_ms);
    duration_ms += delay_ms;
  }
  /// Calculates the index of the texture that should be displayed.
  /// @param ellapsed_ms The total number of ellapsed milliseconds for the game play.
  /// @returns The index of the texture that should be displayed.
  Index calculate_texture_index(std::size_t ellapsed_ms) const noexcept override {

    ellapsed_ms %= duration_ms;

    decltype(ellapsed_ms) ellapsed_animation = 0;

    for (auto& frame : frames) {
      if ((ellapsed_ms >= ellapsed_animation) && (ellapsed_ms < (ellapsed_animation + frame.delay_ms))) {
        return frame.texture;
      } else {
        ellapsed_animation += frame.delay_ms;
      }
    }

    return Index();
  }
};

} // namespace

Animation* Animation::get_null_animation() noexcept {
  return &null_animation;
}

ScopedPtr<Animation> Animation::make() {
  return new AnimationImpl();
}

ScopedPtr<Animation> Animation::make_single_frame(Index texture) {
  auto animation = Animation::make();
  animation->add_frame(texture, SIZE_MAX);
  return animation;
}

} // namespace herald
