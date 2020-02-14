#include "QtTextureTable.h"

#include "Index.h"
#include "ScopedPtr.h"

#include <QPixmap>

#include <vector>

namespace herald {

namespace {

/// Implements the Qt texture table.
class QtTextureTableImpl final : public QtTextureTable {
  /// The pixel maps for each loaded texture.
  std::vector<QPixmap> pixmaps;
public:
  /// Opens a new texture.
  /// @param filename The path to the texture to open.
  void open(const char* filename) override {
    pixmaps.emplace_back(filename);
  }
  /// Gets the pixmap for a texture at a specified index.
  /// @param index The index to get the texture of.
  /// @returns The pixmap at the specified location.
  QPixmap at(Index index) const override {
    if (index >= pixmaps.size()) {
      return QPixmap();
    } else {
      return pixmaps.at(index);
    }
  }
};

} // namespace

ScopedPtr<QtTextureTable> QtTextureTable::make() {
  return new QtTextureTableImpl;
}

} // namespace herald
