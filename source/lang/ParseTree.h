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

/// This node contains a response from
/// the "add objects" command.
class FillObjectsResponse final {
  /// The matrix containing the indices
  /// for each object actions.
  Matrix* action_matrix;
  /// The matrix containing flags for
  /// each of the objects.
  Matrix* flag_matrix;
public:
  /// Constructs the response instance.
  /// @param a The action matrix to assign.
  /// After calling this function, this response
  /// will take ownership of the pointer.
  /// @param f The flags containing information
  /// about the objects.
  FillObjectsResponse(Matrix* a, Matrix* f);
  /// Releases memory allocated by the response.
  ~FillObjectsResponse();
  /// Accesses the matrix containing the action indices.
  inline const Matrix& get_action_matrix() const noexcept {
    return *action_matrix;
  }
  /// Accesses the matrix used to containg object flags.
  inline const Matrix& get_flag_matrix() const noexcept {
    return *flag_matrix;
  }
};

/// This node contains a response
/// from the "set background" command.
class SetBackgroundResponse final {
  /// The animation to assign the background.
  int animation;
  /// The frame to start the animation at.
  int frame;
public:
  /// Constructs an instance of the response.
  /// @param a The animation assigned.
  /// @param f The starting frame.
  constexpr SetBackgroundResponse(int a, int f) noexcept
    : animation(a), frame(f) {}
  /// Accesses the index of the animation.
  /// @returns The animation to assign the background.
  int get_animation() const noexcept {
    return animation;
  }
  /// Accesses the frame to start the animation at.
  /// @returns The frame to start the animation at.
  int get_frame() const noexcept {
    return frame;
  }
};
