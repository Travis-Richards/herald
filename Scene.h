#pragma once

#include <QGraphicsScene>

class QColor;
class QPointF;
class QSize;
class QString;
class TextureList;

/// A scene contains the data to be rendered.
/// It may, for example, be a menu or a playable level.
class Scene : public QGraphicsScene {
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
  /// The list of textures used for the game.
  TextureList* textures;
};
