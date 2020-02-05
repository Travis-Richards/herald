#include "Scene.h"

#include "TextureList.h"

#include <QSize>

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
  textures = TextureList::make(this);
}

Scene::~Scene() {

}

void Scene::draw_box(const QPointF& a, const QPointF& b, int texture_id) {
  (void)a;
  (void)b;
  (void)texture_id;
}

void Scene::clear() {

}

void Scene::load_color_texture(const QColor& color) {
  (void)color;
}

void Scene::load_image_texture(const QString& path) {
  (void)path;
}

void Scene::set_background_texture(int id) {
  (void)id;
}

void Scene::set_view_size(const QSize& view_size) {
  (void)view_size;
}
