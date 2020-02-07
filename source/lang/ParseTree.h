#pragma once

class Matrix;
class QSize;

/// This node contains a response from
/// the "build room" command.
class BuildRoomResponse {
public:
  /// Constructs an instance of the response.
  /// @param textures The texture matrix.
  /// @param frames The frame offset matrix.
  /// @param flags The flag matrix.
  /// @returns A new response instance.
  static BuildRoomResponse* make(Matrix* textures,
                                 Matrix* frames,
                                 Matrix* flags);
  /// Just a stub.
  virtual ~BuildRoomResponse() {}
  /// Accesses the matrix containing the tile texture indices.
  virtual const Matrix& get_texture_matrix() const noexcept = 0;
  /// Accesses the matrix containing the tile frame offsets.
  virtual const Matrix& get_frame_matrix() const noexcept = 0;
  /// Accesses the matrix containing the flags for each tile.
  virtual const Matrix& get_flag_matrix() const noexcept = 0;
};
