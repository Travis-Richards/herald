#include "QtObjectTable.h"

#include "QtObject.h"
#include "ScopedPtr.h"

#include <QGraphicsItemGroup>

#include <vector>

namespace herald {

namespace {

/// The implementation of the Qt object table.
class QtObjectTableImpl final : public QtObjectTable {
  /// The graphics item group to put the objects into.
  ScopedPtr<QGraphicsItemGroup> item_group;
  /// The objects added to the object table.
  std::vector<ScopedPtr<QtObject>> objects;
  /// The standard reference size for all objects.
  QSize standard_size;
public:
  /// Constructs a new instance of the Qt object table.
  /// @param parent A pointer to the parent graphics item.
  QtObjectTableImpl(QGraphicsItem* parent) : item_group(new QGraphicsItemGroup(parent)), standard_size(1, 1) {
    item_group->setZValue(1);
  }
  /// Accesses an object at a specific index.
  Object* at(Index index) override {
    if (index >= objects.size()) {
      return Object::get_null_object();
    } else {
      return objects[index].get();
    }
  }
  /// Accesses a pointer to the graphics item.
  /// @returns A pointer to the graphics item.
  QGraphicsItem* get_graphics_item() override {
    return item_group.get();
  }
  /// Resizes the number of objects in the table.
  /// @param count The number of objects to keep in the table.
  void resize(std::size_t count) {

    /* Handle collapse */
    if (count < objects.size()) {
      objects.resize(count);
    }

    /* Handle expansion */
    for (std::size_t i = objects.size(); i < count; i++) {
      objects.emplace_back(QtObject::make(item_group.get()));
    }
  }
  /// Resizes the standard reference size for all objects.
  /// @param size The new standard size to adjust to.
  void resize_standard(const QSize& size) override {

    standard_size = size;

    for (auto& obj : objects) {
      if (obj) {
        obj->resize(standard_size);
      }
    }
  }
  /// Indicates the number of items
  /// in the object table.
  std::size_t size() const noexcept override {
    return objects.size();
  }
  /// Updates the object positions.
  void update_positions(const QSize& tile_size) override {
    for (auto& obj : objects) {
      if (obj) {
        obj->update_position(tile_size);
      }
    }
  }
  /// Updates the texture indices assigned to each of the objects.
  /// @param ellapsed_ms The total number of ellapsed milliseconds.
  /// @param animations A reference to the animation table.
  void update_texture_indices(std::size_t ellapsed_ms, const AnimationTable& animations) override {
    for (auto& obj : objects) {
      obj->update_texture_index(ellapsed_ms, animations);
    }
  }
  /// Updates the textures assigned to each of the objects.
  void update_textures(const QtTextureTable& textures) override {
    for (auto& obj : objects) {
      obj->update_texture(textures);
    }
  }
};

} // namespace

ScopedPtr<QtObjectTable> QtObjectTable::make(QGraphicsItem* parent) {
  return new QtObjectTableImpl(parent);
}

} // namespace herald
