#pragma once

#include <cstddef>

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
  /// Indicates the size of the texture table.
  /// @returns The size of the texture table.
  virtual std::size_t size() const noexcept = 0;
};

} // namespace herald
