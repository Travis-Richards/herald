#include <herald/sysinfo/Registry.h>

#include <herald/ScopedPtr.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdlib>

namespace herald {

namespace sysinfo {

namespace {

#ifdef _WIN32

/// The implementation of the registry interface.
class RegistryImpl final : public Registry {
public:
  /// Accesses a value from the registry.
  /// @param specifier The value specifier.
  /// @param value A reference to a variable to receive the value.
  bool get(const Specifier& specifier, std::uint32_t& value) override {

    auto key = to_native_key(specifier.key());

    DWORD tmp_value = 0;

    DWORD tmp_value_size = sizeof(tmp_value);

    auto ret = get_value(specifier,
                         RRF_RT_REG_DWORD,
                         nullptr,
                         &tmp_value,
                         &tmp_value_size);

    auto success = (ret == ERROR_SUCCESS);

    if (success) {
      value = tmp_value;
    }

    return success;
  }
  /// Gets a string value from the registry.
  /// @param specifier The value specifier.
  /// @returns A string containing the specified value.
  String get(const Specifier& specifier) override {

    DWORD data_size = 0;

    auto ret = get_value(specifier,
                         RRF_RT_REG_SZ,
                         nullptr,
                         nullptr,
                         &data_size);

    if (ret != ERROR_SUCCESS) {
      return String::make_invalid();
    }

    String out(data_size);

    ret = get_value(specifier,
                    RRF_RT_REG_SZ,
                    nullptr,
                    out.get_data_pointer(),
                    &data_size);

    if (ret != ERROR_SUCCESS) {
      return String::make_invalid();
    }

    return out;
  }
protected:
  /// Converts the key enumeration
  /// to the Windows native key handle.
  HKEY to_native_key(RegistryKey key) {

    switch (key) {
      case RegistryKey::CurrentUser:
        return HKEY_CURRENT_USER;
      case RegistryKey::LocalMachine:
        return HKEY_LOCAL_MACHINE;
    }

    // Should be unreachable

    return HKEY_LOCAL_MACHINE;
  }
  /// A wrapper around RegGetValueA.
  /// @param spec The value specifier.
  /// @param flags Flags for the access call.
  /// @param type Used to get type information on the data.
  /// @param data A pointer to the data to put the value in.
  /// @param data_size A pointer to get the data size.
  /// @returns The error code of the operation.
  LONG get_value(const Specifier& spec,
                 DWORD flags,
                 LPDWORD type,
                 PVOID data,
                 LPDWORD data_size) {

    return ::RegGetValueA(to_native_key(spec.key()),
                          spec.sub_key(),
                          spec.value(),
                          flags,
                          type,
                          data,
                          data_size);
  }
};

#else

/// Used in non-Windows platforms.
class NullRegistry final : public Registry {
public:
  /// Does nothing.
  /// @returns Always returns false.
  bool get(const Specifier&, std::uint32_t&) override {
    return false;
  }
  /// Does nothing.
  /// @returns An empty string.
  String get(const Specifier&) override {
    return String::make_invalid();
  }
};

#endif

} // namespace

RegistryString::RegistryString(std::size_t s) {
  if (s > 0) {
    data = (char*) std::calloc(1, (s + 1));
    max = data ? s : 0;
  } else {
    data = nullptr;
    max = 0;
  }
}

RegistryString::RegistryString(RegistryString&& other) {

  data = other.data;
  max = other.max;

  other.data = nullptr;
  other.max = 0;
}

RegistryString::~RegistryString() {
  std::free(data);
}

ScopedPtr<Registry> Registry::make() {
#ifdef _WIN32
  return new RegistryImpl();
#else
  return new NullRegistry();
#endif
}

} // namespace sysinfo

} // namespace herald
