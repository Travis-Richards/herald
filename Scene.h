#pragma once

#include <Qt3DCore/QEntity>

class TextureList;
class QColor;
class QString;

/// A scene contains the data to be rendered.
/// It may, for example, be a menu or a playable level.
class Scene : public Qt3DCore::QEntity {
  Q_OBJECT
public:
  /// Adds a polygon object to the scene.
  /// @param polygon The polygon to add to the scene.
  /// @param texture_id The ID of the texture to map onto the polygon.
  void add_polygon_object(const QPolygonF& polygon, int texture_id);
  /// Clears the contents in the scene.
  void clear();
  /// Creates a brush out of a color and
  /// allows it to be addressed as a texture.
  void load_color_texture(const QColor& color);
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
