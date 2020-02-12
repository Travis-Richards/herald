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
  virtual void add(const Object& object) = 0;
  /// Gets an object at a specified index.
  /// @param index The index of the object to get.
  /// @returns On success, a pointer to the object.
  /// On failure, a null pointer.
  virtual const Object* at(int index) const noexcept = 0;
  /// Gets the number of objects
  /// in the object map.
  virtual int size() const noexcept = 0;
};
