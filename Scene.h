#pragma once

#include <Qt3DCore/QEntity>

class TextureList;
class QColor;
class QString;

/// A scene contains the data to be rendered.
/// It may, for example, be a menu or a playable level.
class Scene : public QObject {
  Q_OBJECT
public:
  /// Constructs a scene instance.
  /// @param parent An optional pointer to a parent object.
  Scene(QObject* parent = nullptr);
  /// Releases memory allocated by the scene.
  ~Scene();
  /// Adds a polygon object to the scene.
  /// @param polygon The polygon to add to the scene.
  /// @param texture_id The ID of the texture to map onto the polygon.
  void add_polygon_object(const QPolygonF& polygon, int texture_id);
  /// Clears the contents in the scene.
  void clear();
  /// Accesse a pointer to the root entity of the scene.
  inline Qt3DCore::QEntity* get_root_entity() noexcept {
    return root_entity;
  }
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
  /// A pointer to the root entity of the scene.
  Qt3DCore::QEntity* root_entity;
  /// The textures loaded for the scene.
  TextureList* textures;
};
