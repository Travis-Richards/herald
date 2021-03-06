#include "QtTile.h"

#include <herald/Animation.h>
#include <herald/AnimationTable.h>
#include <herald/Index.h>
#include <herald/ScopedPtr.h>

#include "QtTextureTable.h"

#include <QGraphicsRectItem>
#include <QPen>
#include <QPixmap>

namespace herald {

namespace {

/// The implementation of the Qt tile interface.
class QtTileImpl final : public QtTile {
  /// The graphics rectangle item.
  ScopedPtr<QGraphicsRectItem> item;
  /// The index of the current texture that the tile is displaying.
  Index texture_index;
  /// The currently displayed texture.
  QPixmap texture;
public:
  /// Constructs an instance of the tile.
  /// @param parent A pointer to the parent graphics item.
  QtTileImpl(QGraphicsItem* parent) : item(new QGraphicsRectItem(parent)) {
    item->setPen(Qt::NoPen);
    item->setBrush(QBrush(QColor(0, 0, 0, 0)));
  }
  /// Accesses a pointer to the graphics item.
  QGraphicsItem* get_graphics_item() override {
    return item.get();
  }
  /// Assigns the rectangle shape to the tile.
  void set_rect(const QRectF& rect) override {
    item->setRect(rect);
    update_brush();
  }
  /// Updates the texture index for the tile.
  /// @param ellapsed_ms The new total ellapsed seconds.
  /// @param animations A reference to the animation table
  /// to get the texture index from.
  /// @returns True if the index changed, false otherwise.
  bool update_texture_index(std::size_t ellapsed_ms, const AnimationTable& animations) override {

    auto* animation = animations.at(get_animation_index());

    auto next_texture_index = animation->calculate_texture_index(ellapsed_ms);

    auto changed = texture_index != next_texture_index;

    texture_index = next_texture_index;

    return changed;
  }
  /// Updates the texture being displayed.
  /// @param textures The table to get the texture from.
  void update_texture(const QtTextureTable& textures) override {
    texture = textures.at(texture_index);
    update_brush();
  }
  /// Updates the brush used to paint the tile.
  void update_brush() {
    if (texture.isNull()) {
      item->setBrush(QColor(0, 0, 0, 0));
    } else {
      item->setBrush(texture.scaled(calculate_pixmap_size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
  }
  /// Calculates the size that the pixmap should get scaled to.
  QSize calculate_pixmap_size() const {
    auto rect = item->rect();
    return QSize(rect.width(), rect.height());
  }
};

} // namespace

ScopedPtr<QtTile> QtTile::make(QGraphicsItem* parent) {
  return new QtTileImpl(parent);
}

} // namespace herald
