#pragma once

#include <QGraphicsScene>

class TextureList;
class QString;

/// A scene contains the data to be rendered.
/// It may, for example, be a menu or a playable level.
class Scene : public QGraphicsScene {
  Q_OBJECT
public:
  /// Constructs the scene.
  /// @param parent A pointer to the parent object.
  Scene(QObject* parent = nullptr);
  /// Loads an image for the scene to use.
  /// @param path The path of the image to load.
  void load_image_texture(const QString& path);
  /// Sets the background image of the scene.
  /// @param texture_id The ID of the texture
  /// to assign as the background.
  void set_background_texture(int texture_id);
private:
  /// The textures loaded for the scene.
  TextureList* textures;
};
