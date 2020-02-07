#pragma once

#include <QObject>

class TextureAnimation;

/// The base interface of a texture animation list.
class TextureAnimationGroup : public QObject {
  Q_OBJECT
public:
  /// Creates a new instance of the texture animation list.
  /// @param parent A pointer to the parent object.
  /// @returns A new texture list instance.
  static TextureAnimationGroup* make(QObject* parent);
  /// Constructs the base of the texture animation list.
  TextureAnimationGroup(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~TextureAnimationGroup() {}
  /// Opens a texture for the list at a specified path.
  /// @param path The path of the texture animation to open.
  /// @returns True on success, false on failure.
  virtual bool open(const QString& path) = 0;
  /// Accesses a texture animation at a specific index.
  /// @param index The index of the texture animation to get.
  /// @returns A pointer to the texture animation at the specified index.
  /// If the index is out of bounds, then a null pointer is returned instead.
  virtual TextureAnimation* at(int index) noexcept = 0;
  /// Accesses a constant pointer to a texture animation at a specific index.
  /// @param index The index of the texture animation to access.
  /// @returns On success, a pointer to the texture animation.
  /// On failure, a null pointer.
  virtual const TextureAnimation* at(int index) const noexcept = 0;
  /// Sets the target frames per second of the animation.
  /// The default value is 30.
  /// @param fps The target frames per second to assign.
  virtual void set_target_fps(int fps) = 0;
  /// Indicates the size of the texture list.
  virtual int size() const noexcept = 0;
  /// Begins the animation of the textures in the group.
  /// @returns True if the animation was started,
  /// false if it was already running.
  virtual bool start() = 0;
  /// Stops the animation.
  virtual void stop() = 0;
signals:
  /// Emitted when all texture animations are in sync.
  /// This is the signal to handle before redrawing the scene.
  void synced();
  /// Updates the active frame for a texture animation.
  /// This is only emitted if the animation group is started.
  /// @param texture The texture animation index.
  /// @param frame The active frame for the texture animation.
  void update_frame(int texture, int frame);
};
