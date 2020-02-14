#pragma once

namespace herald {

/// The base class for a texture table.
/// Used by @ref CpuModelImpl
class TextureTable {
public:
  /// Just a stub.
  virtual ~TextureTable() {}
  /// Opens a new texture.
  /// @param filename The path to the texture to open.
  virtual void open(const char* filename) = 0;
};

} // namespace herald
