#pragma once

class Matrix;
class QSize;

/// This node contains a response from
/// the "build room" command.
class BuildRoomResponse {
public:
  /// Constructs an instance of the response.
  /// @param tm The texture matrix.
  /// @param fm The flag matrix.
  /// @returns A new response instance.
  static BuildRoomResponse* make(Matrix* tm, Matrix* fm);
  /// Just a stub.
  virtual ~BuildRoomResponse() {}
  /// Accesses the matrix containing the tile texture indices.
  virtual const Matrix& get_texture_matrix() const noexcept = 0;
  /// Accesses the matrix containing the flags for each tile.
  virtual const Matrix& get_flag_matrix() const noexcept = 0;
};
