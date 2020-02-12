#pragma once

class QJsonValue;

/// Represents an arbitrary action
/// that an object can do.
class Action final {
  /// The index of the animation
  /// to render for this action.
  int animation;
public:
  /// Constructs the default action.
  constexpr Action() noexcept : animation(-1) {}
  /// Accesses the index of the animation.
  inline int get_animation() const noexcept {
    return animation;
  }
  /// Reads action data from a JSON value.
  /// @param value The JSON value to get the data from.
  /// @returns True on success, false on failure.
  bool read(const QJsonValue& value);
};
