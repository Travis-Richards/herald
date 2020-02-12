#include "ObjectMap.h"

#include "Action.h"
#include "ActionList.h"
#include "Object.h"

#include <QVector>

namespace {

/// Implements the object map interface.
class ObjectMapImpl final : public ObjectMap {
  /// The objects part of the map.
  QVector<Object> objects;
public:
  /// Constructs the object map implementation.
  /// @param parent A pointer to the parent Qt object.
  ObjectMapImpl(QObject* parent) : ObjectMap(parent) { }
  /// Adds an object to the map.
  /// @param object The object to add to the map.
  void add(const Object& object) override {
    objects.push_back(object);
  }
  /// Accesses an object at a specific index.
  const Object* at(int index) const noexcept override {
    if (index < 0) {
      return nullptr;
    } else if (index >= objects.size()) {
      return nullptr;
    } else {
      return &objects[index];
    }
  }
  /// Maps the object actions.
  void map_actions(const ActionList* actions) noexcept override {
    auto count = actions->size();
    for (int i = 0; i < count; i++) {
      map_action(i, actions->at(i));
    }
  }
  /// Updates the object animations.
  void update_animations(int animation, int frame) noexcept override {
    for (auto& obj : objects) {
      if (obj.has_animation(animation)) {
        obj.update_frame(frame);
      }
    }
  }
  /// Accesses the number of objects in the map.
  int size() const noexcept override {
    return objects.size();
  }
protected:
  /// Maps a single action onto the objects.
  void map_action(int index, const Action* action) noexcept {
    for (auto& obj : objects) {
      if (obj.has_action(index)) {
        obj.map_action(index, action);
      }
    }
  }
};

} // namespace

ObjectMap* ObjectMap::make(QObject* parent) {
  return new ObjectMapImpl(parent);
}
