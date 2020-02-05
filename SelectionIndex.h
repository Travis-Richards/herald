#pragma once

/// A selection index class is a wrapper for
/// an integer representing an index. The index
/// is considered "invalid" until it is explicity assigned.
class SelectionIndex final {
  /// The value of the index.
  int value;
public:
  /// Constructs the selection index instance.
  constexpr SelectionIndex() noexcept
    : value(-1) {}
  /// Indicates wether or not the index is invalid.
  constexpr bool invalid() const noexcept {
    return value < 0;
  }
  /// Indicates whether or not the index is valid.
  constexpr bool valid() const noexcept {
    return value >= 0;
  }
  /// Assigns a value to the selection index.
  /// @param v The value to assign.
  SelectionIndex& operator = (int v) noexcept {
    value = v;
    return *this;
  }
  /// Casts the class to its integer value.
  inline operator int () const noexcept {
    return value;
  }
};
