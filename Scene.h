#pragma once

#include <QGraphicsScene>

class Matrix;
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
  /// Builds a room from a texture matrix.
  /// @param texture_matrix A matrix containing the
  /// texture indices for each tile in the room.
  virtual void build_room(const Matrix& texture_matrix) = 0;
  /// Loads a texture to be used by the scene.
  /// @param path A path to the texture to open.
  virtual void load_texture(const QString& path) = 0;
public slots:
  /// Handles the resizing of the video that's being drawn to.
  /// @param size The new size of the scene.
  virtual void resize(const QSize& size) = 0;
};
