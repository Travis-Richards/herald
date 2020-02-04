#pragma once

#include <QObject>

class MaterialList;
class QColor;
class QPointF;
class QSize;
class QString;

namespace Qt3DCore {

class QEntity;
class QTransform;

} // namespace Qt3DCore

namespace Qt3DExtras {

class QPlaneMesh;

} // namespace Qt3DExtras

namespace Qt3DRender {

class QMaterial;

} // namespace Qt3DRender

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
  /// Clears the contents in the scene.
  void clear();
  /// Draws an axis-aligned box onto the scene.
  /// @param a The first point of the box.
  /// @param b The second point of the box.
  /// @param texture_id The ID of the texture to assign.
  void draw_box(const QPointF& a, const QPointF& b, int texture_id);
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
public slots:
  /// Sets the resolution of the scene view.
  /// This is used to determine mesh resolutions and sizes.
  void set_view_size(const QSize& view_size);
private:
  /// A pointer to the root entity of the scene.
  Qt3DCore::QEntity* root_entity;
  /// The entity for the background.
  Qt3DCore::QEntity* background_entity;
  /// The plane mesh for the background.
  Qt3DExtras::QPlaneMesh* background_plane;
  /// The material assigned to the background.
  Qt3DRender::QMaterial* background_material;
  /// The transformation matrix for the background plane.
  Qt3DCore::QTransform* background_transform;
  /// A temporary entity for building scenes.
  /// Anything not related to the background
  /// plane is added to this entity so that it
  /// the content can be removed easily.
  Qt3DCore::QEntity* tmp_entity;
  /// The materials loaded for the scene.
  MaterialList* materials;
};
