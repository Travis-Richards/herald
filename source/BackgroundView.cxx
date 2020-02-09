#include "BackgroundView.h"

#include "Background.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "TextureAnimationGroup.h"

#include <QBrush>

namespace {

/// The implementation of the background view.
class BackgroundViewImpl final : public BackgroundView {
public:
  /// Constructs an instance of the background view implementation.
  /// @param parent A pointer to the parent graphics info.
  BackgroundViewImpl(QGraphicsItem* parent) : BackgroundView(parent) {
    setZValue(-1);
  }
  /// Maps the background data into the view.
  /// @param background The background data to map.
  /// @param animations The animation group containing the map-able frames.
  void map(const Background& background, const TextureAnimationGroup& animations) override {
    setBrush(get_brush(background, animations));
  }
  /// Resizes the background view.
  /// @param size The size to adjust the background view to.
  void resize_view(const QSize& size) override {
    setRect(0, 0, size.width(), size.height());
  }
protected:
  /// Gets the brush to be used for a mapping.
  /// @param background The background data.
  /// @param animations The game's animation group.
  /// @returns The resultant brush. If the background
  /// data could not be retrieved, then the default
  /// black brush is used.
  QBrush get_brush(const Background& background, const TextureAnimationGroup& animations) {

    const auto* animation = animations.at(background.get_animation());
    if (!animation) {
      return QBrush();
    }

    const auto* texture = animation->get_frame(background.get_frame());
    if (!texture) {
      return QBrush();
    }

    auto background_size = QSize(rect().width(), rect().height());

    return texture->as_brush(background_size);
  }
};

} // namespace

BackgroundView* BackgroundView::make(QGraphicsItem* parent) {
  return new BackgroundViewImpl(parent);
}
