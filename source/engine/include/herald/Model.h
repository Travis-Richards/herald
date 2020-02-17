#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

class ActionTable;
class AnimationTable;
class Background;
class Index;
class ObjectTable;
class Room;
class TextureTable;

/// This is the base for the data model for the game.
class Model {
public:
  /// Just a stub.
  virtual ~Model() {}
  /// Accesses a pointer to the animation table.
  virtual AnimationTable* get_animation_table() = 0;
  /// Accesses a pointer to the action table.
  virtual ActionTable* get_action_table() = 0;
  /// Accesses a pointer to the background instance.
  virtual Background* get_background() = 0;
  /// Accesses a pointer to the object map;
  virtual ObjectTable* get_object_table() = 0;
  /// Accesses a pointer to the model's room.
  virtual Room* get_room() = 0;
  /// Accesses a pointer to the texture table.
  virtual TextureTable* get_texture_table() = 0;
};

} // namespace herald
