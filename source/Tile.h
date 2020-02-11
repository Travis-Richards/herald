#pragma once

/// Represents one tile within a room.
class Tile {
public:
  /// Constructs the tile instance.
  /// @param parent A pointer to the parent object.
  constexpr Tile() noexcept : mod_flag(true), animation(0), frame(0) { }
  /// Changes the animation used by the tile.
  void change_animation(int index) noexcept {
    mod_flag = true;
    animation = index;
  }
  /// Updates the frame index of the animation.
  void change_frame(int index) noexcept {
    mod_flag = frame != index;
    frame = index;
  }
  /// Clears the modification flag.
  void clear_mod_flag() noexcept {
    mod_flag = false;
  }
  /// Gets the index of the frame that
  /// this tile is currently at.
  int get_frame() const noexcept {
    return frame;
  }
  /// Gets the index of the texture animation
  /// assigned to this tile.
  int get_animation() const noexcept {
    return animation;
  }
  /// Indicates whether or not the tile has a particular texture.
  bool has_animation(int a) const noexcept {
    return animation == a;
  }
  /// Indicates whether or not the tile
  /// was modified.
  bool modified() const noexcept {
    return mod_flag;
  }
  /// Manually assigns the mod flag.
  /// This should only be done rarely to
  /// force a cache miss. As of writing this,
  /// the only use for this function is to
  /// force a redraw operation.
  void set_mod_flag() noexcept {
    mod_flag = true;
  }
private:
  /// A modification flag, used to that
  /// tiles are only drawn when modified.
  bool mod_flag;
  /// The index of the texture animation
  /// assigned to this tile.
  int animation;
  /// The frame that the texture animation
  /// for the tile is currently at.
  int frame;
};
