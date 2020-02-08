#include "ObjectMap.h"

#include <vector>

namespace {

/// Implements the object map interface.
class ObjectMapImpl final : public ObjectMap {
  /// The objects part of the map.
  std::vector<Object*> objects;
public:
  /// Constructs the object map implementation.
  /// @param parent A pointer to the parent Qt object.
  ObjectMapImpl(QObject* parent) : ObjectMap(parent) {}
  /// Adds an object to the map.
  /// @param object The object to add to the map.
  void add(Object* object) override {
    objects.emplace_back(object);
  }
};

} // namespace

ObjectMap* ObjectMap::make(QObject* parent) {
  return new ObjectMapImpl(parent);
}
