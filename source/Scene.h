#pragma once

#include <QGraphicsScene>

class Background;
class Room;
class ObjectMap;
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
  /// Accesses the background of the scene for modification.
  virtual Background* get_background() = 0;
  /// Accesses the scene's object map for modification.
  virtual ObjectMap* get_object_map() = 0;
  /// Accesses the scene's room for modification.
  virtual Room* get_room() = 0;
  /// Loads a texture to be used by the scene.
  /// @param path A path to the texture to open.
  virtual void load_texture(const QString& path) = 0;
  /// Updates the view of the scene.
  /// This includes the room view and
  /// the object map view.
  /// Not to be confused with Qt's QGraphicsView.
  virtual void update_view() = 0;
  /// Starts the scene's animation sequence.
  virtual void start() = 0;
  /// Pauses the scenes animation sequence.
  virtual void pause() = 0;
public slots:
  /// Handles the resizing of the video that's being drawn to.
  /// @param size The new size of the scene.
  virtual void resize(const QSize& size) = 0;
};
