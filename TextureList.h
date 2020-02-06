#pragma once

#include <QObject>

class Texture;

/// The base interface of a texture list.
class TextureList : public QObject {
public:
  /// Creates a new instance of the texture list.
  /// @param parent A pointer to the parent object.
  /// @returns A new texture list instance.
  static TextureList* make(QObject* parent);
  /// Constructs the base of the texture list.
  TextureList(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~TextureList() {}
  /// Adds a new texture to the list.
  /// @param texture A pointer to the texture to add.
  virtual void add(Texture* texture) = 0;
  /// Accesses a texture at a specific index.
  /// @param index The index of the texture to get.
  /// @returns A pointer to the texture at the specified index.
  /// If the index is out of bounds, then a null pointer is returned instead.
  virtual Texture* at(int index) noexcept = 0;
  /// Indicates the size of the texture list.
  virtual int size() const noexcept = 0;
};
