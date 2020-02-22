#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace sysinfo {

class RegistrySpecifier;

/// Used for locating programs on the system.
class ProgramFinder {
public:
#ifdef _WIN32
  /// The default path separator for Windows.
  static constexpr char default_separator = ';';
  /// The default extension used by Windows executables.
  static constexpr const char* default_ext = ".exe";
#else
  /// The default path separator for non-Windows.
  static constexpr char default_separator = ':';
  /// All non-Windows platforms don't have a
  /// default extension for executable programs.
  static constexpr const char* default_ext = "";
#endif
  /// Creates a new program finder instance.
  /// @returns A new program finder instance.
  static ScopedPtr<ProgramFinder> make();
  /// Just a stub.
  virtual ~ProgramFinder() {}
  /// Adds an environment list variable.
  /// The environment variable, PATH, is a good example
  /// of an environment list variable.
  /// @param name The of the variable to add.
  /// @param separator The separator used by the variable.
  virtual void add_env_list(const char* name, char separator = default_separator) = 0;
  /// Adds a list of paths to be searched by the program finder.
  /// @param list The list of paths to add.
  /// The paths in this list must be separated by @p separator.
  /// @param separator The path separator.
  virtual void add_path_list(const char* list, char separator) = 0;
  /// Adds a path from a registry value.
  /// @param specifier The specifier for the registry value to add.
  virtual void add_registry_path(const RegistrySpecifier& specifier) = 0;
  /// Attempts to find a program.
  /// All paths added to the program finder instance are searched.
  /// @param basename The basename of the program to search for.
  /// This should not include an extension.
  /// @param ext The extension to append to the basename,
  /// when searching for the program.
  /// @returns True if the program was found, false if it was not.
  virtual bool find(const char* name, const char* ext = default_ext) = 0;
  /// Gets the program found in the last "find" operation.
  /// @returns A null-terminated string is always returned by this function.
  /// If a program wasn't found, then the string is empty (but not a null pointer.)
  virtual const char* get_last_result() const noexcept = 0;
};

} // namespace sysinfo

} // namespace herald
