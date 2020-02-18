#pragma once

#include <cstddef>

namespace herald {

template <typename T>
class ScopedPtr;

namespace protocol {

class Token;

/// Used for scanning tokens from a response protocol.
class Lexer {
public:
  /// Creates a new lexer instance.
  /// @param data The data for the lexer to scan.
  /// @param size The number of characters in the data string.
  static ScopedPtr<Lexer> make(const char* data, std::size_t size);
  /// Just a stub.
  virtual ~Lexer() {}
  /// Indicates whether or not the lexer
  /// has reached the end of the input string.
  virtual bool done() const noexcept = 0;
  /// Scans a token from the input string.
  /// @returns A new token instance.
  virtual Token scan() noexcept = 0;
};

} // namespace protocol

} // namespace herald
