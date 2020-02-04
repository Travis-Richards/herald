#pragma once

#include <QObject>

namespace Qt3DRender {

class QMaterial;

} // namespace Qt3DRender

using Material = Qt3DRender::QMaterial;

/// The base interface of a material list.
class MaterialList : public QObject {
public:
  /// Creates a new instance of the material list.
  /// @param parent A pointer to the parent object.
  /// @returns A new material list instance.
  static MaterialList* make(QObject* parent);
  /// Constructs the base of the material list.
  MaterialList(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~MaterialList() {}
  /// Adds a new material to the list.
  /// @param material A pointer to the material to add.
  virtual void add(Material* material) = 0;
  /// Accesses a material at a specific index.
  /// @param index The index of the material to get.
  /// @returns A pointer to the material at the specified index.
  /// If the index is out of bounds, then a null pointer is returned instead.
  virtual Material* at(int index) noexcept = 0;
  /// Indicates the size of the material list.
  virtual int size() const noexcept = 0;
};
