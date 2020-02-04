#include "Scene.h"

#include "MaterialList.h"

#include <QSize>

#include <Qt3DCore/QEntity>

#include <QAbstractTexture>
#include <QTextureMaterial>
#include <QTextureImage>
#include <QUrl>

#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QPlaneMesh>

namespace {

/// Creates the default background material.
/// @param root_entity The root entity of the material being made.
/// @returns A new material instance with the default values.
Qt3DRender::QMaterial* make_default_material(Qt3DCore::QEntity* root_entity) {
  auto* material = new Qt3DExtras::QDiffuseSpecularMaterial(root_entity);
  material->setDiffuse(QColor(16, 16, 16));
  return material;
}

} // namespace

Scene::Scene(QObject* parent) : QObject(parent) {

  root_entity = new Qt3DCore::QEntity();

  background_entity = new Qt3DCore::QEntity(root_entity);

  background_plane = new Qt3DExtras::QPlaneMesh(background_entity);
  background_plane->setHeight(4);
  background_plane->setWidth(4);

  background_material = make_default_material(background_entity);

  background_transform = new Qt3DCore::QTransform(background_entity);
  background_transform->setScale(1);
  background_transform->setRotationX(90);
  background_transform->setTranslation(QVector3D(0, 0, -1));

  background_entity->addComponent(background_plane);
  background_entity->addComponent(background_transform);
  background_entity->addComponent(background_material);

  tmp_entity = new Qt3DCore::QEntity(root_entity);

  materials = MaterialList::make(this);
}

Scene::~Scene() {

}

void Scene::draw_box(const QPointF& a, const QPointF& b, int texture_id) {

  // Adjust points to get proper min/max

  auto min = [](qreal a, qreal b) -> qreal {
    return a < b ? a : b;
  };

  QPointF min_point(min(a.x(), b.x()),
                    min(a.y(), b.y()));

  auto max = [](qreal a, qreal b) -> qreal {
    return a > b ? a : b;
  };

  QPointF max_point(max(a.x(), b.x()),
                    max(a.y(), b.y()));

  auto x_diff = max_point.x() - min_point.x();
  auto y_diff = max_point.y() - min_point.y();

  // Add entity

  auto* entity = new Qt3DCore::QEntity(root_entity);

  auto* plane = new Qt3DExtras::QPlaneMesh(entity);
  plane->setWidth(x_diff);
  plane->setHeight(y_diff);
  entity->addComponent(plane);

  auto* transform = new Qt3DCore::QTransform(entity);
  transform->setRotationX(90);
  transform->setTranslation(QVector3D(min_point.x(), min_point.y(), 0));
  entity->addComponent(transform);

  auto* texture = materials->at(texture_id);
  if (!texture) {
    // TODO : fallback texture
  } else {
    entity->addComponent(texture);
  }
}

void Scene::clear() {

  delete tmp_entity;

  tmp_entity = new Qt3DCore::QEntity(root_entity);
}

void Scene::load_color_texture(const QColor& color) {
  (void)color;
  materials->add(nullptr);
}

void Scene::load_image_texture(const QString& path) {

  auto* material = new Qt3DExtras::QTextureMaterial(background_entity);

  auto* texture = material->texture();

  auto* texture_image = new Qt3DRender::QTextureImage(texture);
  texture_image->setMirrored(false);
  texture_image->setSource(QUrl::fromLocalFile(path));

  texture->addTextureImage(texture_image);

  materials->add(material);
}

void Scene::set_background_texture(int id) {

  background_entity->removeComponent(background_material);

  delete background_material;

  background_material = materials->at(id);

  if (background_material) {
    background_entity->addComponent(background_material);
  }
}

void Scene::set_view_size(const QSize& view_size) {

  qreal h = view_size.height();
  qreal w = view_size.width();
  auto aspect_ratio = w / h;

  if (h > w) {
    background_plane->setHeight(background_plane->height() * aspect_ratio);
  } else {
    background_plane->setWidth(background_plane->width() * aspect_ratio);
  }
}
