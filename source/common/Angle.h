#pragma once

#ifdef _MSC_VER
#define _USE_MATH_DEFINES 1
#endif // _MSC_VER

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace herald {

/// Used to describe angles,
/// without knowing if they're
/// in degrees or radians.
/// @tparam Scalar The type
/// used to contain the angle value.
template <typename Scalar>
class Angle final {
  /// The value of the angle.
  Scalar value;
  /// We don't want unspecified angle values anywhere.
  Angle() = delete;
  /// A private constructor for
  /// the factory methods to call.
  /// @param v The angle value, in radians.
  constexpr Angle(Scalar v) noexcept : value(v) {}

public:
  /// Constructs an angle value in radians.
  /// @param v The value, in radians, to assign the angle.
  static constexpr Angle Radians(Scalar v) noexcept { return Angle(v); }
  /// Constructs an angle value in degrees.
  /// @param v The value, in degrees, to assign the angle.
  static constexpr Angle Degrees(Scalar v) noexcept {
    return Angle((v * M_PI) / 180);
  }
  static constexpr Angle Zero() noexcept { return Angle(0); }
  Scalar sine() const noexcept { return std::sin(value); }
  Scalar cosine() const noexcept { return std::cos(value); }
  Scalar tangent() const noexcept {return std::tan(value); }
  constexpr Angle operator-() const noexcept { return Angle(-value); }
  constexpr Angle operator*(const Scalar& n) const noexcept { return Angle(value * n); }
  constexpr Angle operator/(const Scalar& n) const noexcept { return Angle(value / n); }
};

} // namespace radiation
