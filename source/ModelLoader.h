#pragma once

class QJsonArray;
class QJsonObject;
class QJsonValue;
class QString;

namespace herald {

template <typename T>
class ScopedPtr;

class Action;
class ActionTable;
class Animation;
class AnimationTable;
class TextureTable;
class Model;

/// Used for loading a model from a file.
class ModelLoader final {
public:
  /// Loads actions from a JSON array.
  /// @param model The model to receive the data.
  /// @param root The root object to get the model data from.
  /// @param game_path The path that the game is located at.
  static void load(Model* model, const QJsonObject& root, const QString& game_path);
protected:
  /// Loads actions directly into the action table.
  /// @param actions The action table to load.
  /// @param json_array The JSON array to get the action objects from.
  static void load_actions(ActionTable* actions, const QJsonArray& json_array);
  /// Loads a single action from a JSON object.
  /// @param action_value The action value to load the data from.
  /// @returns A new action instance.
  static Action load_action(const QJsonValue& action_value);
  /// Loads animations into the animation table.
  /// @param json_array The JSON array to get the animation data from.
  static void load_animations(AnimationTable* animations, const QJsonArray& json_array);
  /// Loads a single animation from a JSON value.
  /// @param animation_value The animation value to get the data from.
  /// @returns An initialized animation instance.
  static ScopedPtr<Animation> load_animation(const QJsonValue& animation_value);
  /// Loads the textures from JSON file.
  /// @param textures The texture table to add the textures into.
  /// @param json_value The JSON value to get the textures from.
  /// @param game_path The path that the game is located at.
  static void load_textures(TextureTable* textures, const QJsonValue& json_value, const QString& game_path);
};

} // namespace herald
