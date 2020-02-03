#include "Scene.h"

#include "Texture.h"
#include "TextureList.h"

Scene::Scene(QObject* parent) : QObject(parent) {
  root_entity = new Qt3DCore::QEntity();
  textures = TextureList::make(this);
}

Scene::~Scene() {

}

void Scene::add_polygon_object(const QPolygonF& polygon, int texture_id) {

  (void)polygon;

  const auto* texture = textures->at(texture_id);
  if (!texture) {
    //addPolygon(polygon, QPen(), QBrush());
  } else {
    //addPolygon(polygon, QPen(), texture->get_brush());
  }
}

void Scene::clear() {

  if (root_entity) {
    delete root_entity;
  }

  root_entity = new Qt3DCore::QEntity();
}

void Scene::load_color_texture(const QColor& color) {
  textures->add(Texture::from_color(color, textures));
}

void Scene::load_image_texture(const QString& path) {
  textures->add(Texture::from_image(path, textures));
}

void Scene::set_background_texture(int id) {
  const auto* texture = textures->at(id);
  (void)texture;
  if (!texture) {
    //setBackgroundBrush(QBrush(Qt::black));
  } else {
    //setBackgroundBrush(texture->get_brush());
  }
}
