#include "ParseTree.h"

#include "Matrix.h"

#include <QSize>

namespace {

/// The implementation of the "build room" response.
class BuildRoomResponseImpl : public BuildRoomResponse {
  /// The texture matrix.
  Matrix* texture_matrix;
  /// The tile flag matrix.
  Matrix* flag_matrix;
public:
  /// Constructs an instance of the response.
  BuildRoomResponseImpl(Matrix* tm, Matrix* fm) {
    texture_matrix = tm ? tm : Matrix::make(0, 0);
    flag_matrix    = fm ? fm : Matrix::make(0, 0);
  }
  /// Accesses the flag matrix.
  const Matrix& get_flag_matrix() const noexcept override {
    return *flag_matrix;
  }
  /// Accesses the texture matrix.
  const Matrix& get_texture_matrix() const noexcept override {
    return *texture_matrix;
  }
};

} // namespace

BuildRoomResponse* BuildRoomResponse::make(Matrix* tm, Matrix* fm) {
  return new BuildRoomResponseImpl(tm, fm);
}
