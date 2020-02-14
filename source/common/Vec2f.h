#pragma once

namespace herald {

/// A simple 2D vector class.
class Vec2f final {
  /// The X component of the vector.
  float x_value;
  /// The Y component of the vector.
  float y_value;
public:
  /// Constructs a zero vector.
  constexpr Vec2f() noexcept : x_value(0), y_value(0) {}
  /// Constructs an initialized vector.
  /// @param a The first component to assign.
  /// @param b The second component to assign.
  constexpr Vec2f(float a, float b) noexcept : x_value(a), y_value(b) {}
  /// Accesses the X component of the vector.
  inline constexpr float x() const noexcept {
    return x_value;
  }
  /// Accesses the Y component of the vector.
  inline constexpr float y() const noexcept {
    return y_value;
  }
  /// Adds two vectors together.
  Vec2f operator + (const Vec2f& other) const noexcept {
    return Vec2f(x_value + other.x_value,
                 y_value + other.y_value);
  }
  /// Subtracts two vectors.
  Vec2f operator - (const Vec2f& other) const noexcept {
    return Vec2f(x_value - other.x_value,
                 y_value - other.y_value);
  }
  /// Multiplies the vector by a scalar value.
  Vec2f operator * (float f) const noexcept {
    return Vec2f(x_value * f, y_value * f);
  }
};

} // namespace herald
