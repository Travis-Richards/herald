#include "Matrix.h"

#include <herald/ScopedPtr.h>

#include <herald/protocol/ParseTree.h>

#include <QSize>

#include <vector>

using namespace herald;

namespace {

/// Implements the matrix interface.
class MatrixImpl final : public Matrix {
  /// The list containing the matrix values.
  std::vector<int> vec;
  /// The width of the matrix.
  std::size_t w;
  /// The height of the matrix.
  std::size_t h;
public:
  /// Constructs an instance of the matrix.
  MatrixImpl(std::size_t width, std::size_t height) : w(width), h(height) {
    auto count = w * h;
    for (std::size_t i = 0; i < count; i++) {
      vec.push_back(0);
    }
  }
  /// Accesses a value within the matrix.
  int at(std::size_t x, std::size_t y) const noexcept override {
    if ((x < w) && (y < h)) {
      return vec[(y * w) + x];
    } else {
      return 0;
    }
  }
  /// Assigns a value within the matrix.
  void set(std::size_t x, std::size_t y, int n) noexcept override {
    if ((x < w) && (y < h)) {
      vec[(y * w) + x] = n;
    }
  }
  /// Accesses the width of the matrix.
  std::size_t width() const noexcept override {
    return w;
  }
  /// Accesses the height of the matrix.
  std::size_t height() const noexcept override {
    return h;
  }
};

} // namespace

ScopedPtr<Matrix> Matrix::make(const protocol::Matrix& m) {

  auto size = m.get_size();
  if (!size.valid()) {
    return Matrix::make(0, 0);
  }

  auto w_node = size.get_width();
  auto h_node = size.get_height();

  int w_signed = 0;
  int h_signed = 0;

  if (!w_node.to_signed_value(w_signed)
   || !h_node.to_signed_value(h_signed)) {
    return Matrix::make(0, 0);
  }

  if ((w_signed < 0) || (h_signed < 0)) {
    return Matrix::make(0, 0);
  }

  std::size_t w = (std::size_t) w_signed;
  std::size_t h = (std::size_t) h_signed;

  auto matrix = Matrix::make(w, h);

  for (std::size_t y = 0; y < h; y++) {

    for (std::size_t x = 0; x < w; x++) {

      auto v_node = m.get_integer((y * w) + x);

      int v = 0;

      if (v_node.to_signed_value(v)) {
        matrix->set(x, y, v);
      }
    }
  }

  return matrix;
}

ScopedPtr<Matrix> Matrix::make(std::size_t w, std::size_t h) {
  return ScopedPtr<Matrix>(new MatrixImpl(w, h));
}

ScopedPtr<Matrix> Matrix::make(const QSize& size) {
  return Matrix::make(size.width(), size.height());
}
