#include "Scene.h"

#include "Texture.h"
#include "TextureList.h"

Scene::Scene(QObject* parent) : QGraphicsScene(parent) {
  textures = TextureList::make(this);
  setBackgroundBrush(QBrush(Qt::black));
  setSceneRect(0, 0, 2000, 2000);
}

void Scene::add_polygon_object(const QPolygonF& polygon, int texture_id) {

  const auto* texture = textures->at(texture_id);
  if (!texture) {
    addPolygon(polygon, QPen(), QBrush());
  } else {
    addPolygon(polygon, QPen(), texture->get_brush());
  }
}

void Scene::load_color_texture(const QColor& color) {
  textures->add(Texture::from_color(color, textures));
}

void Scene::load_image_texture(const QString& path) {
  textures->add(Texture::from_image(path, textures));
}

void Scene::set_background_texture(int id) {
  const auto* texture = textures->at(id);
  if (!texture) {
    setBackgroundBrush(QBrush(Qt::black));
  } else {
    setBackgroundBrush(texture->get_brush());
  }
}
