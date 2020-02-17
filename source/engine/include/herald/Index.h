#pragma once

#include <cstddef>

#ifndef SIZE_MAX
#define SIZE_MAX 0xffff
#endif

namespace herald {

/// A wrapper around a index value
/// so that it's always properly initialized.
class Index final {
  std::size_t value;
public:
  /// Constructs an index.
  inline constexpr Index() noexcept : value(SIZE_MAX) {}
  /// Constructs an index with a specified value.
  /// @param v The value to assign the index.
  inline constexpr Index(std::size_t v) noexcept : value(v) {}
  /// Indicates whether or not the index is valid.
  inline bool valid() const noexcept {
    return value != SIZE_MAX;
  }
  /// Indicates whether or not the index is invalid.
  inline bool invalid() const noexcept {
    return value == SIZE_MAX;
  }
  /// Casts the index back to a size value.
  inline operator std::size_t () const noexcept {
    return value;
  }
};

} // namespace herald
