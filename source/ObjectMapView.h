#pragma once

#include <QGraphicsItemGroup>

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
  /// Resizes the view of the object map.
  /// @param size The new window size to adjust to.
  virtual void resize_view(const QSize& size) = 0;
};
