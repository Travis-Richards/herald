#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace parse_tree { class Matrix; }

} // namespace herald

class QSize;

/// A matrix is used to specify texture
/// indices and other characteristics of the room tiles.
class Matrix {
public:
  /// Creates a matrix from a parser tree
  /// representation of a matrix.
  static herald::ScopedPtr<Matrix> make(const herald::parse_tree::Matrix& m);
  /// Creates a new matrix instance.
  /// @param width The width of the matrix.
  /// @param height The height of the matrix.
  static herald::ScopedPtr<Matrix> make(int width, int height);
  /// Creates a new matrix.
  static herald::ScopedPtr<Matrix> make(const QSize& size);
  /// Just a stub.
  virtual ~Matrix() {}
  /// Accesses a number within the matrix.
  /// @returns The number at the specified coordinates.
  /// If the coordinates are out of bounds, then zero
  /// is returned instead.
  virtual int at(int x, int y) const noexcept = 0;
  /// Sets a number within the matrix.
  /// @param n The number to assign at the specified coordinates.
  virtual void set(int x, int y, int n) noexcept = 0;
  /// Indicates the width of the matrix.
  virtual int width() const noexcept = 0;
  /// Indicates the height of the matrix.
  virtual int height() const noexcept = 0;
};
