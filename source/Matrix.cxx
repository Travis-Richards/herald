#include "Matrix.h"

#include "ScopedPtr.h"

#include "protocol/ParseTree.h"

#include <QList>
#include <QSize>

using namespace herald;

namespace {

/// Implements the matrix interface.
class MatrixImpl final : public Matrix {
  /// The list containing the matrix values.
  QList<int> vec;
  /// The width of the matrix.
  int w;
  /// The height of the matrix.
  int h;
public:
  /// Constructs an instance of the matrix.
  MatrixImpl(int width, int height) : w(width), h(height) {
    auto count = w * h;
    for (int i = 0; i < count; i++) {
      vec.push_back(0);
    }
  }
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

ScopedPtr<Matrix> Matrix::make(const parse_tree::Matrix& m) {

  auto size = m.get_size();
  if (!size.valid()) {
    return Matrix::make(0, 0);
  }

  auto w_node = size.get_width();
  auto h_node = size.get_height();

  int w = 0;
  int h = 0;

  if (!w_node.to_signed_value(w)
   || !h_node.to_signed_value(h)) {
    return Matrix::make(0, 0);
  }

  if ((w < 0) || (h < 0)) {
    return Matrix::make(0, 0);
  }

  auto matrix = Matrix::make(w, h);

  for (int y = 0; y < h; y++) {

    for (int x = 0; x < w; x++) {

      auto v_node = m.get_integer((y * w) + x);

      int v = 0;

      if (v_node.to_signed_value(v)) {
        matrix->set(x, y, v);
      }
    }
  }

  return matrix;
}

ScopedPtr<Matrix> Matrix::make(int w, int h) {
  return ScopedPtr<Matrix>(new MatrixImpl(w, h));
}

ScopedPtr<Matrix> Matrix::make(const QSize& size) {
  return Matrix::make(size.width(), size.height());
}
