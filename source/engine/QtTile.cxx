#include "QtTile.h"

#include "Animation.h"
#include "AnimationTable.h"
#include "Index.h"
#include "ScopedPtr.h"
#include "QtTextureTable.h"

#include <QGraphicsRectItem>
#include <QPen>
#include <QPixmap>

#include <iostream>

namespace herald {

namespace {

/// The implementation of the Qt tile interface.
class QtTileImpl final : public QtTile {
  /// The graphics rectangle item.
  ScopedPtr<QGraphicsRectItem> item;
  /// The animation queue.
  std::vector<Index> animation_queue;
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
  /// Gets the index of the current animation.
  /// @returns The index of the current animation.
  Index get_current_animation() const noexcept {
    if (animation_queue.empty()) {
      return Index();
    } else {
      return animation_queue[0];
    }
  }
  /// Accesses a pointer to the graphics item.
  QGraphicsItem* get_graphics_item() override {
    return item.get();
  }
  /// Queues a change in animation.
  void queue_animation(Index animation) override {
    animation_queue.push_back(animation);
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
  void update_texture_index(std::size_t ellapsed_ms, const AnimationTable& animations) override {

    auto* animation = animations.at(get_current_animation());

    texture_index = animation->calculate_texture_index(ellapsed_ms);
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
