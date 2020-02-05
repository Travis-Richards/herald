#pragma once

#include <QGraphicsScene>

class QSize;
class QString;

/// A scene contains the data to be rendered.
/// It may, for example, be a menu or a playable level.
class Scene : public QGraphicsScene {
  Q_OBJECT
public:
  /// Constructs a full scene instance.
  /// @param parent A pointer to the parent object.
  /// @returns A pointer to a new scene instance.
  static Scene* make(QObject* parent);
  /// Constructs the base scene instance.
  /// @param parent An optional pointer to a parent object.
  Scene(QObject* parent = nullptr) : QGraphicsScene(parent) {}
  /// Just a stub.
  virtual ~Scene() {}
  /// Sets the size of the map.
  /// This indicates how many tiles there
  /// will be horizontally and vertically.
  /// @param size The size to assign the map,
  /// in terms of tile counts.
  virtual void set_map_size(const QSize& size) = 0;
  /// Assigns the size of one level, in terms of tiles.
  /// @param level_size Used to indicate the width and
  /// height, in terms of tiles.
  virtual void set_level_size(const QSize& level_size) = 0;
  /// Loads a texture to be used by the scene.
  /// @param path A path to the texture to open.
  virtual void load_texture(const QString& path) = 0;
};
