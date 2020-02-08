#include "TextureAnimationGroup.h"

#include "TextureAnimation.h"

#include <QList>
#include <QTimer>

namespace {

/// Calculates the index of the frame to use for
/// a texture animation at a specific moment in time.
/// @param texture_animation The texture animation to get the frame index of.
/// @param ellapsed_ms The total number of ellapsed milliseconds.
int calc_frame_index(const TextureAnimation* texture_animation, int ellapsed_ms) {

  ellapsed_ms %= texture_animation->get_total_duration();

  int total = texture_animation->get_frame_count();

  int ellapsed_animation = 0;

  for (int i = 0; i < total; i++) {

    if (ellapsed_animation >= ellapsed_ms) {
      return i;
    }

    ellapsed_animation += texture_animation->get_frame_duration(i);
  }

  return 0;
}

/// Implements the texture animation list interface.
class TextureAnimationGroupImpl final : public TextureAnimationGroup {
  /// The actual list of texture animations.
  QList<TextureAnimation*> textures;
  /// A timer for frame animation.
  QTimer timer;
  /// The total number of ellapsed milliseconds.
  int ellapsed_ms;
public:
  /// Constructs an instance of the texture list implementation.
  TextureAnimationGroupImpl(QObject* parent) : TextureAnimationGroup(parent) { 

    ellapsed_ms = 0;

    connect(&timer, &QTimer::timeout, this, &TextureAnimationGroupImpl::update);

    set_target_fps(30);
  }
  /// Opens a texture animation at a specified path.
  /// @param path The path to the texture animation to open.
  /// @returns True on success, false on failure.
  bool open(const QString& path) override {
    textures.push_back(TextureAnimation::open(path, this));
    return true;
  }
  /// Accesses a texture at a specified index.
  TextureAnimation* at(int index) noexcept override {
    if (index < 0) {
      return nullptr;
    } else if (index < textures.size()) {
      return textures[index];
    } else {
      return nullptr;
    }
  }
  /// Accesses a texture at a specified index.
  const TextureAnimation* at(int index) const noexcept override {
    if (index < textures.size()) {
      return textures[index];
    } else {
      return nullptr;
    }
  }
  /// Sets the target frames per second.
  /// @param fps The target frames per second.
  void set_target_fps(int fps) override {
    timer.setInterval(1000 / fps);
  }
  /// Indicates the size of the texture list.
  int size() const noexcept override {
    return textures.size();
  }
  /// Begins the animation of the texture group.
  /// @returns True if the animation was started,
  /// false if it was alreadyt active.
  bool start() override {

    if (timer.isActive()) {
      return false;
    }

    ellapsed_ms = 0;

    timer.start();

    return true;
  }
  /// Stops the animation.
  void stop() override {
    timer.stop();
  }
protected slots:
  /// Updates the animation instances.
  void update() {

    ellapsed_ms += timer.interval();

    for (int i = 0; i < textures.size(); i++) {
      emit update_frame(i, calc_frame_index(textures.at(i), ellapsed_ms));
    }

    emit synced();
  }
};

} // namespace

TextureAnimationGroup* TextureAnimationGroup::make(QObject* parent) {
  return new TextureAnimationGroupImpl(parent);
}
