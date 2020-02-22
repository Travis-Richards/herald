#pragma once

#include <cstddef>
#include <cstdint>

namespace herald {

template <typename T>
class ScopedPtr;

namespace sysinfo {

/// Enumerates the predefined
/// handles for registry keys.
enum class RegistryKey : int {
  /// Used for information that is user specific.
  CurrentUser,
  /// Used for information that is system specific.
  LocalMachine
};

/// A registry specifier is used to
/// locate data in the registry.
class RegistrySpecifier final {
  /// The predefined key being used.
  RegistryKey key_;
  /// The sub key for the specifier.
  const char* sub_key_;
  /// The name of the value being retrieved.
  const char* value_;
public:
  /// Constructs a new instance of a specifier.
  /// @param k The predefined registry key to use.
  /// @param sk The subkey to use.
  /// This may be a null pointer.
  /// @param v The name of the value to get.
  constexpr RegistrySpecifier(RegistryKey k,
                              const char* sk,
                              const char* v) noexcept
    : key_(k), sub_key_(sk), value_(v) {}
  /// Accesses the key specifier.
  inline RegistryKey key() const noexcept {
    return key_;
  }
  /// Accesses the sub-key specifier.
  inline const char* sub_key() const noexcept {
    return sub_key_;
  }
  /// Accesses the name of the value.
  inline const char* value() const noexcept {
    return value_;
  }
};

/// Used to contain a string from the Windows registry.
class RegistryString final {
  /// A pointer to the string data.
  char* data;
  /// The number of characters in the string.
  std::size_t size_;
public:
  /// Creates an "invalid" registry string.
  /// @returns A registry string with no data.
  static RegistryString make_invalid() {
    return RegistryString(0);
  }
  /// Creates a new registry string.
  /// @param s The number of characters to allocate
  /// for the string data.
  RegistryString(std::size_t s);
  /// Constructs a string via move semantics.
  /// @param other The string to pull the data from.
  RegistryString(RegistryString&& other);
  /// Delete copy constructors.
  RegistryString(const RegistryString&) = delete;
  RegistryString& operator = (const RegistryString&) = delete;
  /// Releases memory allocated by the string.
  ~RegistryString();
  /// Gets a pointer to the actual string data.
  /// @returns A pointer to the actual string data.
  inline const char* c_str() const noexcept {
    return data;
  }
  /// Gets a pointer to the string data.
  inline char* get_data_pointer() noexcept {
    return data;
  }
  /// Indicates whether or not the string is valid.
  inline bool valid() const noexcept {
    return data != nullptr;
  }
  /// Accesses the string size.
  /// This is the number of characters in the string,
  /// not including the null terminating character.
  inline std::size_t size() const noexcept {
    return size_;
  }
  /// Accesses a beginning iterator.
  const char* begin() const noexcept {
    return data;
  }
  /// Accesses an ending iterator.
  const char* end() const noexcept {
    return data + size_;
  }
  /// Accesses a character in the registry string.
  /// @param index The index of the character to get.
  /// @returns The specified character on success,
  /// a value of zero in the case that the index of out of bounds.
  inline char operator [] (std::size_t index) const noexcept {
    return (index < size_) ? data[index] : 0;
  }
};

/// This is an interface to the Windows
/// registry. On non-Windows platforms,
/// an instance of this class can be made
/// but it does nothing in the underlying
/// implementation.
class Registry {
public:
  /// A simple type definition for registry keys.
  using Key = RegistryKey;
  /// A simple type definition for value specifiers.
  using Specifier = RegistrySpecifier;
  /// A simple type definition for registry strings.
  using String = RegistryString;
  /// Creates a new instance of the registry interface.
  /// @returns A new instance of the registry interface.
  static ScopedPtr<Registry> make();
  /// Just a stub.
  virtual ~Registry() {}
  /// Accesses a double word value from the registry.
  /// @param specifier The specifier for the value getting retrieved.
  /// @param value A reference to the variable that will receive the value.
  /// @returns True on success, false on failure.
  virtual bool get(const Specifier& specifier, std::uint32_t& value) = 0;
  /// Accesses a string value from the registry.
  /// @param specifier The specifier for the value to get.
  virtual String get(const Specifier& specifier) = 0;
};

} // namespace sysinfo

} // namespace herald
