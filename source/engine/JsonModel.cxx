#include <herald/JsonModel.h>

#include <herald/ScopedPtr.h>

#include "Json.h"

#include <fstream>
#include <sstream>

namespace herald {

namespace {

using Json = nlohmann::json;

/// The implementation of the JSON model.
class JsonModelImpl final : public JsonModel {
  /// The JSON instance containing the model data.
  Json json;
public:
  /// Reads a JSON model from a stream.
  /// @param input The stream to read the model from.
  JsonModelImpl(std::istream& input) {
    input >> json;
  }
  /// Accesses a pointer to the animation table.
  AnimationTable* get_animation_table() override {
    return nullptr;
  }
  /// Accesses a pointer to the action table.
  ActionTable* get_action_table() override {
    return nullptr;
  }
  /// Accesses a pointer to the background instance.
  Background* get_background() override {
    return nullptr;
  }
  /// Accesses a pointer to the object map;
  ObjectTable* get_object_table() override {
    return nullptr;
  }
  /// Accesses a pointer to the model's room.
  Room* get_room() override {
    return nullptr;
  }
  /// Accesses a pointer to the texture table.
  TextureTable* get_texture_table() override {
    return nullptr;
  }
};

} // namespace

ScopedPtr<JsonModel> JsonModel::open(const char* filename) {

  std::ifstream file(filename);

  return new JsonModelImpl(file);
}

ScopedPtr<JsonModel> JsonModel::from_source(const char* source) {

  std::istringstream stream(source);

  return new JsonModelImpl(stream);
}

} // namespace herald
