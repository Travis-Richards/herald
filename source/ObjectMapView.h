#pragma once

#include <QGraphicsItemGroup>

class ActionList;
class ObjectMap;
class TextureAnimationGroup;

/// A graphical item set of the items
/// in the scene that are controllable.
class ObjectMapView : public QGraphicsItemGroup {
public:
  /// Creates a new instance of an object map view.
  /// @param parent A pointer to the parent graphics item,
  /// if one exists. This parameter may be a null pointer.
  static ObjectMapView* make(QGraphicsItem* parent);
  /// Constructs a view of the object map.
  /// @param parent A pointer to the parent graphics item.
  ObjectMapView(QGraphicsItem* parent) : QGraphicsItemGroup(parent) {}
  /// Just a stub.
  virtual ~ObjectMapView() {}
  /// Maps the texture animations onto the object map view.
  /// @param object_map The object map data.
  /// @param animations The animations to map onto the object.
  virtual void map(const ObjectMap& object_map,
                   const TextureAnimationGroup& animations) = 0;
  /// Resizes the boundaries of the map.
  /// @param width The new width of the map.
  /// @param height The new height of the map.
  virtual void resize(int weight, int height) = 0;
  /// Resizes the view of the object map.
  /// @param size The new window size to adjust to.
  virtual void resize_view(const QSize& size) = 0;
};
