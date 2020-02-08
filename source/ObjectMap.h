#pragma once

#include <QObject>

class Object;

/// Used for tracing objects
/// within a room.
class ObjectMap : public QObject {
public:
  /// Creates a new object map instance.
  /// @param parent A pointer to the parent object.
  static ObjectMap* make(QObject* parent);
  /// Constructs a new object map instance.
  /// @param parent A pointer to the parent object.
  ObjectMap(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~ObjectMap() {}
  /// Adds an object to the object map.
  /// @param object The object to add.
  virtual void add(Object* object) = 0;
};
