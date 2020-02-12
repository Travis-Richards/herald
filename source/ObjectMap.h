#pragma once

#include <QObject>

class ActionList;
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
  /// Maps the animations defined by actions onto the objects.
  /// Should be called only after responses that can alter actions.
  virtual void map_actions(const ActionList* actions) noexcept = 0;
  /// Updates the frame of an animation.
  /// @param animation The index of the animation to update.
  /// @param frame The frame to assign the animation.
  virtual void update_animations(int animation, int frame) noexcept = 0;
  /// Gets the number of objects
  /// in the object map.
  virtual int size() const noexcept = 0;
};
