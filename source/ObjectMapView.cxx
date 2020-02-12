#include "ObjectMapView.h"

#include "Object.h"
#include "ObjectMap.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "TextureAnimationGroup.h"

#include <QBrush>
#include <QGraphicsRectItem>
#include <QPen>
#include <QSize>
#include <QVector>

namespace {

/// Gets the minimum of two values.
template <typename T>
T min(T a, T b) noexcept {
  return (a < b) ? a : b;
}

/// A view of a single object within the map.
class ObjectView final : public QGraphicsRectItem {
  /// The X position of the object, in terms of tile location.
  double x;
  /// The Y position of the object, in terms of tile location.
  double y;
public:
  /// Constructs an instance of the object view.
  /// @param parent A pointer to the parent graphics item.
  ObjectView(QGraphicsItem* parent) : QGraphicsRectItem(parent) {
    setPen(QPen(Qt::NoPen));
    x = 0;
    y = 0;
  }
  /// Accesses the X position.
  inline float get_x() const noexcept {
    return x;
  }
  /// Accesses the Y position.
  inline float get_y() const noexcept {
    return y;
  }
  /// Assigns a X value.
  inline void set_x(float x_) noexcept {
    x = x_;
  }
  /// Assigns a Y value.
  inline void set_y(float y_) noexcept {
    y = y_;
  }
};

/// The implementation of the object map view interface.
class ObjectMapViewImpl final : public ObjectMapView {
  /// The width of the map.
  int w;
  /// The height of the map.
  int h;
  /// The object views of the map.
  QVector<ObjectView*> object_views;
  /// The size of the window that
  /// the objects are being viewed on.
  QSize window_size;
public:
  /// Constructs an instance of the object map view implementation.
  ObjectMapViewImpl(QGraphicsItem* parent) : ObjectMapView(parent) {
    setZValue(1);
    w = 0;
    h = 0;
  }
  /// Maps texture animations to the object map.
  /// @param object_map The object map data.
  /// @param textures The textures to map onto the objects.
  void map(const ObjectMap& object_map, const TextureAnimationGroup& textures) override {

    for (auto i = object_views.size(); i < object_map.size(); i++) {
      object_views.push_back(new ObjectView(this));
    }

    auto count = object_views.size();

    for (int i = 0; i < count; i++) {
      map_object(object_views[i], object_map.at(i), textures);
    }
  }
  /// Resizes the boundaries of the map.
  /// @param width The new width of the map.
  /// @param height The new height of the map.
  void resize(int width, int height) override {
    w = width;
    h = height;
  }
  /// Resizes a view of the object map.
  /// @param size The window size to adjust the view to.
  void resize_view(const QSize& size) override {

    auto tile_size = calc_tile_size();

    for (auto* object_view : object_views) {

      auto x = object_view->get_x();
      auto y = object_view->get_y();

      object_view->setRect(x * tile_size.width(),
                           y * tile_size.height(),
                           tile_size.width(),
                           tile_size.height());
    }

    window_size = size;
  }
protected:
  /// Calculates the size of a tile.
  QSize calc_tile_size() const {
    auto object_w = w ? (window_size.width()  / w) : 0;
    auto object_h = h ? (window_size.height() / h) : 0;
    return QSize(object_w, object_h);
  }
  /// Maps a single object onto the view.
  /// @param object_view The object view to update.
  /// @param object The object to map.
  /// @param animations The animations to be used for the object.
  bool map_object(ObjectView* object_view, const Object* object, const TextureAnimationGroup& animations) {

    if (!object) {
      return false;
    }

    auto* animation = animations.at(object->get_animation());
    if (!animation) {
      return false;
    }

    auto* texture = animation->get_frame(object->get_frame());
    if (!texture) {
      return false;
    }

    auto tile_size = calc_tile_size();

    auto x = object->get_x();
    auto y = object->get_y();

    auto object_rect = QRectF(x * tile_size.width(),
                              y * tile_size.height(),
                              tile_size.width(),
                              tile_size.height());

    object_view->setBrush(texture->as_brush(tile_size));

    object_view->setRect(object_rect);

    return true;
  }
};

} // namespace

ObjectMapView* ObjectMapView::make(QGraphicsItem* parent) {
  return new ObjectMapViewImpl(parent);
}
