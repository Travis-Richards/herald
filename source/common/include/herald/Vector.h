#pragma once

#include <vector>

namespace herald {

/// This class is a simple wrapper around
/// a STL vector so that it can be forward declared.
/// @tparam T The type of the element to store.
template <typename T>
class Vector final {
  /// The actual STL vector.
  std::vector<T> vec;
public:
  /// Constructs an empty vector.
  Vector() {}
  /// Constructs a vector via copy.
  /// @param other The vector to copy.
  Vector(const Vector<T>& other) : vec(other.vec) {}
  /// Constructs a vector using move semantics.
  /// @param other The vector to move the data from.
  Vector(Vector<T>&& other) : vec(std::move(other.vec)) {}
  /// Accesses an element at a specific index.
  /// @param index The index to accesses the element at.
  /// @returns A reference to the specified element.
  inline T& at(std::size_t index) {
    return vec.at(index);
  }
  /// Accesses an element at a specific index.
  /// @param index The index to accesses the element at.
  inline const T& at(std::size_t index) const {
    return vec.at(index);
  }
  /// Accesses the beginning iterator for the vector.
  typename std::vector<T>::const_iterator begin() const noexcept {
    return vec.begin();
  }
  /// Moves an element to the back of the vector.
  /// @param element The element to move.
  inline void emplace_back(T&& element) {
    vec.emplace_back(std::move(element));
  }
  /// Accesses the ending iterator for the vector.
  typename std::vector<T>::const_iterator end() const noexcept {
    return vec.end();
  }
  /// Adds an element to the back of the vector.
  /// @param element The element to add.
  inline void push_back(const T& element) {
    vec.push_back(element);
  }
  /// Indicates the size of the vector.
  /// @returns The size of the vector.
  inline std::size_t size() const noexcept {
    return vec.size();
  }
};

} // namespace herald
