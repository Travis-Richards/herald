#include "QtObject.h"

#include <herald/ScopedPtr.h>
#include <herald/Vec2f.h>

#include "QtTextureTable.h"

#include <QGraphicsRectItem>
#include <QPen>

namespace herald {

namespace {

/// The implementation of the Qt object interface.
class QtObjectImpl final : public QtObject {
  /// A pointer to the graphical representation of the item.
  ScopedPtr<QGraphicsRectItem> item;
public:
  /// Constructs a new instance of the Qt object.
  /// @param parent A pointer to the parent graphics item.
  QtObjectImpl(QGraphicsItem* parent) : item(new QGraphicsRectItem(parent)) {
    item->setPen(QPen(Qt::NoPen));
    item->setRect(0, 0, 1, 1);
  }
  /// Resizes the object to a new standard reference size.
  /// @param standard_size The standard reference size to adjust to.
  void resize(const QSize& standard_size) override {

    /// Currently not using scale factors. If
    /// we were, then this function would account for
    /// them.

    auto item_rect = item->rect();
    int w_initial = item_rect.width();
    int h_initial = item_rect.height();

    int w_final = standard_size.width();
    int h_final = standard_size.height();

    auto size_changed = (w_initial != w_final)
                     || (h_initial != h_final);

    if (!size_changed) {
      return;
    }

    float w_factor = ((float) w_final) / w_initial;
    float h_factor = ((float) h_final) / h_initial;

    item->setRect(w_factor * item_rect.x(),
                  h_factor * item_rect.y(),
                  w_factor * item_rect.width(),
                  h_factor * item_rect.height());
  }
  /// Updates the scene position of the object.
  /// @param tile_size The size of a tile, used for reference.
  void update_position(const QSize& tile_size) override {

    auto pos = get_position();

    auto item_rect = item->rect();
    auto w = item_rect.width();
    auto h = item_rect.height();

    auto x = pos.x() * tile_size.width();
    auto y = pos.y() * tile_size.height();

    item->setRect(QRectF(x, y, w, h));
  }
  /// Updates the texture used to display the object.
  void update_texture(const QtTextureTable& textures) override {

    auto pixmap = textures.at(get_texture_index());

    if (!pixmap.isNull()) {

      auto item_rect = item->rect();

      auto item_size = QSize(item_rect.width(), item_rect.width());

      item->setBrush(pixmap.scaled(item_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
  }
};

} // namespace

ScopedPtr<QtObject> QtObject::make(QGraphicsItem* parent) {
  return new QtObjectImpl(parent);
}

} // namespace herald
