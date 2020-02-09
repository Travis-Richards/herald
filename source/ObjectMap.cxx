#include "ObjectMap.h"

#include <QList>

namespace {

/// Implements the object map interface.
class ObjectMapImpl final : public ObjectMap {
  /// The objects part of the map.
  QList<Object*> objects;
public:
  /// Constructs the object map implementation.
  /// @param parent A pointer to the parent Qt object.
  ObjectMapImpl(QObject* parent) : ObjectMap(parent) { }
  /// Adds an object to the map.
  /// @param object The object to add to the map.
  void add(Object* object) override {
    objects.push_back(object);
  }
  /// Accesses an object at a specific index.
  const Object* at(int index) const noexcept override {
    if (index < 0) {
      return nullptr;
    } else if (index >= objects.size()) {
      return nullptr;
    } else {
      return objects[index];
    }
  }
  /// Accesses the number of objects in the map.
  int size() const noexcept override {
    return objects.size();
  }
};

} // namespace

ObjectMap* ObjectMap::make(QObject* parent) {
  return new ObjectMapImpl(parent);
}
