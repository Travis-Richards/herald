#include "Matrix.h"

#include "ScopedPtr.h"

#include <QSize>

#include <vector>

namespace {

/// Implements the matrix interface.
class MatrixImpl final : public Matrix {
  /// The vector containing the matrix values.
  std::vector<int> vec;
  /// The width of the matrix.
  int w;
  /// The height of the matrix.
  int h;
public:
  /// Constructs an instance of the matrix.
  MatrixImpl(int width, int height)
    : vec(width * height),
      w(width),
      h(height) { }
  /// Accesses a value within the matrix.
  int at(int x, int y) const noexcept override {
    if ((x < w) && (y < h)) {
      return vec[(y * w) + x];
    } else {
      return 0;
    }
  }
  /// Assigns a value within the matrix.
  void set(int x, int y, int n) noexcept override {
    if ((x < w) && (y < h)) {
      vec[(y * w) + x] = n;
    }
  }
  /// Accesses the width of the matrix.
  int width() const noexcept override {
    return w;
  }
  /// Accesses the height of the matrix.
  int height() const noexcept override {
    return h;
  }
};

} // namespace

ScopedPtr<Matrix> Matrix::make(int w, int h) {
  return ScopedPtr<Matrix>(new MatrixImpl(w, h));
}

ScopedPtr<Matrix> Matrix::make(const QSize& size) {
  return Matrix::make(size.width(), size.height());
}
