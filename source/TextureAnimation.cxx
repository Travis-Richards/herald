#include "TextureAnimation.h"

#include "Texture.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QList>
#include <QMovie>
#include <QPair>
#include <QPixmap>
#include <QStringList>

#include <limits>

namespace {

/// Implements the texture animation interface.
class TextureAnimationImpl final : public TextureAnimation {
  /// The frames of animation.
  QList<QPair<Texture*, int>> frames;
  /// The total duration, in milliseconds.
  int total_duration_ms;
public:
  /// Constructs an instance of the texture animation implementation.
  /// @param parent A pointer to the parent object.
  TextureAnimationImpl(QObject* parent) : TextureAnimation(parent) {
    total_duration_ms = 0;
  }
  /// Adds a frame to the animation.
  /// @param frame The frame to add to the animation.
  /// @param delay The delay of the frame in milliseconds.
  void add(Texture* frame, int delay) {
    frames.push_back(QPair<Texture*, int>(frame, delay));
    total_duration_ms += delay;
  }
  /// Opens a directory containing frames for the animation.
  /// @param path The path to the directory to open.
  /// @returns True on success, false on failure.
  bool open_dir(const QString& path);
  /// Opens a gif file and imports the frames into the animation instance.
  /// @param path The path to the GIF file to open.
  /// @returns True on success, false on failure.
  bool open_gif(const QString& path);
  /// Opens a single image file.
  /// @param path The path to the file to open.
  /// @returns True on success, false on failure.
  bool open_single(const QString& path);
  /// Accesses a frame at a specified index.
  /// @returns A pointer to the frame's texture.
  /// If the animation has no frames, then a null
  /// pointer is returned instead.
  Texture* get_frame(int index) noexcept override {

    if (!frames.size()) {
      return nullptr;
    }

    return frames[index % frames.size()].first;
  }
  /// Accesses a frame at a specified index.
  /// @returns A pointer to the frame's texture.
  /// If the texture has no frames, then a null pointer is returned.
  const Texture* get_frame(int index) const noexcept override {

    if (!frames.size()) {
      return nullptr;
    } else if (index < 0) {
      return nullptr;
    }

    return frames[index % frames.size()].first;
  }
  /// Accesses the number of frames in the animation.
  int get_frame_count() const noexcept override {
    return frames.size();
  }
  /// Accesses the number of frames per second.
  int get_frame_duration(int index) const noexcept override {
    if (index >= frames.size()) {
      return std::numeric_limits<int>::max();
    } else {
      return frames[index].second;
    }
  }
  /// Accesses the total duration of the texture.
  int get_total_duration() const noexcept override {
    return total_duration_ms;
  }
protected:
  /// Gets a list of entries in a directory.
  /// @param path The path to the directory.
  /// @returns A list of filenames within the directory.
  static QStringList get_dir_entries(const QString& path);
};

bool TextureAnimationImpl::open_dir(const QString& path) {

  // TODO : get frames per second

  auto files = get_dir_entries(path);

  for (auto file : files) {
    add(Texture::open(file, this), 33);
  }

  return true;
}

bool TextureAnimationImpl::open_gif(const QString& path) {

  QMovie movie(path);

  if (!movie.isValid()) {
    return false;
  }

  for (int i = 0; i < movie.frameCount(); i++) {

    if (!movie.jumpToFrame(i)) {
      return false;
    }

    add(Texture::make(movie.currentPixmap(), this), movie.nextFrameDelay());
  }

  return true;
}

bool TextureAnimationImpl::open_single(const QString& path) {
  add(Texture::open(path, this), std::numeric_limits<int>::max());
  return true;
}

QStringList TextureAnimationImpl::get_dir_entries(const QString& path) {

  QStringList entries;

  QDirIterator it(path, QDir::Files);

  while (it.hasNext()) {
    entries << it.next();
  }

  entries.sort();

  return entries;
}

} // namespace

TextureAnimation* TextureAnimation::open(const QString& path, QObject* parent) {

  auto* texture_animation = new TextureAnimationImpl(parent);

  QFileInfo file_info(path);

  if (file_info.isDir()) {
    texture_animation->open_dir(path);
  } else if (file_info.suffix() == "gif") {
    texture_animation->open_gif(path);
  } else {
    texture_animation->open_single(path);
  }

  return texture_animation;
}
