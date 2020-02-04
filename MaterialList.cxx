#include "MaterialList.h"

#include <QList>

namespace {

/// Implements the material list interface.
class MaterialListImpl final : public MaterialList {
  /// The actual list of materials.
  QList<Material*> materials;
public:
  /// Constructs an instance of the material list implementation.
  MaterialListImpl(QObject* parent) : MaterialList(parent) {}
  /// Adds a material to the material list.
  void add(Material* material) override {
    materials.push_back(material);
  }
  /// Accesse a material at a specified index.
  Material* at(int index) noexcept override {
    if (index < materials.size()) {
      return materials[index];
    } else {
      return nullptr;
    }
  }
  /// Indicates the size of the material list.
  int size() const noexcept override {
    return materials.size();
  }
};

} // namespace

MaterialList* MaterialList::make(QObject* parent) {
  return new MaterialListImpl(parent);
}
