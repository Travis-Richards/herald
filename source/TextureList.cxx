#include "TextureList.h"

#include <QList>

namespace {

/// Implements the texture list interface.
class TextureListImpl final : public TextureList {
  /// The actual list of textures.
  QList<Texture*> textures;
public:
  /// Constructs an instance of the texture list implementation.
  TextureListImpl(QObject* parent) : TextureList(parent) {}
  /// Adds a texture to the texture list.
  void add(Texture* texture) override {
    textures.push_back(texture);
  }
  /// Accesse a texture at a specified index.
  Texture* at(int index) noexcept override {
    if (index < textures.size()) {
      return textures[index];
    } else {
      return nullptr;
    }
  }
  /// Indicates the size of the texture list.
  int size() const noexcept override {
    return textures.size();
  }
};

} // namespace

TextureList* TextureList::make(QObject* parent) {
  return new TextureListImpl(parent);
}
