#pragma once

#include <QObject>

class QString;
class Texture;

/// A texture animation is a series of textures
/// that can be played in sequence.
class TextureAnimation : public QObject {
public:
  /// Opens a texture animation.
  /// @param path The path of the animation to open.
  /// This may either be an animated file or a directory with animation frames.
  /// @param parent A pointer to the parent object to assign.
  static TextureAnimation* open(const QString& path, QObject* parent);
  /// Constructs the base animation sequence.
  /// @param parent A pointer to the parent object.
  TextureAnimation(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~TextureAnimation() {}
  /// Gets a frame at a specified index.
  /// @param index The index to get the frame of.
  /// If this index is greater than the total number
  /// of frames in the animation, then it wraps back
  /// ground to the beginning frame.
  /// @returns A pointer to the frame's texture.
  /// If the animation has no frames, then a null
  /// pointer is returned instead.
  virtual Texture* get_frame(int index) noexcept = 0;
  /// Gets the number of frames avaiable in the animation.
  /// @returns The number of frames avaiable in the animation.
  virtual int get_frame_count() const noexcept = 0;
  /// Gets the duration of a specified frame (in milliseconds.)
  /// @param index The index of the frame to get the duration of.
  /// @returns The duraction of the specified frame, in milliseconds.
  virtual int get_frame_duration(int index) const noexcept = 0;
  /// Gets the total duration of the texture animation.
  virtual int get_total_duration() const noexcept = 0;
};
