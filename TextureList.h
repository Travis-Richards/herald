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
  /// Adds a texture to the texture list.
  /// @param texture The texture to add to the list.
  virtual void add(Texture* texture) = 0;
  /// Gets a texture at a specified index.
  /// @param index The index of the texture to get.
  /// @returns If @p index is within bounds,
  /// then a pointer to the texture is returned.
  /// If it's out of bounds, then a null pointer
  /// is returned instead.
  virtual const Texture* at(int index) const = 0;
  /// Indicates the size of the texture list.
  virtual int size() const noexcept = 0;
};
