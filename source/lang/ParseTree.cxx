#include "ParseTree.h"

#include "Matrix.h"

#include <QSize>

namespace {

/// The implementation of the "build room" response.
class BuildRoomResponseImpl : public BuildRoomResponse {
  /// The texture matrix.
  Matrix* texture_matrix;
  /// The frame offset matrix.
  Matrix* frame_matrix;
  /// The tile flag matrix.
  Matrix* flag_matrix;
public:
  /// Constructs an instance of the response.
  BuildRoomResponseImpl(Matrix* textures,
                        Matrix* frames,
                        Matrix* flags) {

    texture_matrix = textures ? textures : Matrix::make(0, 0);
    frame_matrix   = frames   ? frames   : Matrix::make(0, 0);
    flag_matrix    = flags    ? flags    : Matrix::make(0, 0);
  }
  /// Accesses the flag matrix.
  const Matrix& get_flag_matrix() const noexcept override {
    return *flag_matrix;
  }
  const Matrix& get_frame_matrix() const noexcept override {
    return *frame_matrix;
  }
  /// Accesses the texture matrix.
  const Matrix& get_texture_matrix() const noexcept override {
    return *texture_matrix;
  }
};

} // namespace

BuildRoomResponse* BuildRoomResponse::make(Matrix* textures,
                                           Matrix* frames,
                                           Matrix* flags) {

  return new BuildRoomResponseImpl(textures, frames, flags);
}
