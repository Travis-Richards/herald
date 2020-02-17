#include "ModelLoader.h"

#include <herald/Action.h>
#include <herald/ActionTable.h>
#include <herald/Animation.h>
#include <herald/AnimationTable.h>
#include <herald/TextureTable.h>
#include <herald/Model.h>
#include <herald/ScopedPtr.h>

#include <QDir>
#include <QJsonArray>
#include <QJsonObject>

#include <climits>

namespace herald {

void ModelLoader::load(Model* model, const QJsonObject& root, const QString& game_path) {

  load_actions(model->get_action_table(), root["actions"].toArray());

  load_animations(model->get_animation_table(), root["animations"].toArray());

  load_textures(model->get_texture_table(), root["textures"].toArray(), game_path);
}

void ModelLoader::load_actions(ActionTable* actions, const QJsonArray& json_array) {
  for (auto json_value : json_array) {
    actions->add(load_action(json_value));
  }
}

Action ModelLoader::load_action(const QJsonValue& action_value) {

  auto action_object = action_value.toObject();

  auto animation_value = action_object["animation"];

  auto animation = animation_value.toInt(INT_MAX);

  return Action((std::size_t) animation);
}

void ModelLoader::load_animations(AnimationTable* animations, const QJsonArray& json_array) {
  for (auto json_value : json_array) {
    animations->add(load_animation(json_value));
  }
}

ScopedPtr<Animation> ModelLoader::load_animation(const QJsonValue& json_value) {

  auto animation = Animation::make();

  for (auto frame_value : json_value.toArray()) {

    auto frame_object = frame_value.toObject();

    auto texture_value = frame_object["texture"];
    auto texture = texture_value.toInt(INT_MAX);

    auto delay_value = frame_object["delay"];
    auto delay = delay_value.toInt(INT_MAX);

    animation->add_frame((std::size_t) texture, (std::size_t)delay);
  }

  return animation;
}

void ModelLoader::load_textures(TextureTable* textures, const QJsonValue& json_value, const QString& game_path) {

  for (auto texture_value : json_value.toArray()) {

    auto texture_path = QDir::cleanPath(game_path + QDir::separator() + texture_value.toString()).toStdString();

    textures->open(texture_path.c_str());
  }
}

} // namespace herald
