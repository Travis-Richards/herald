#include <herald/sysinfo/ProgramFinder.h>

#include <herald/sysinfo/Registry.h>

#include <herald/ScopedPtr.h>

#include <fstream>
#include <string>
#include <vector>

#include <cstdlib>

namespace herald {

namespace sysinfo {

namespace {

/// The implementation of the program finder interface.
class ProgramFinderImpl final : public ProgramFinder {
  /// A list of paths to find programs at.
  std::vector<std::string> path_list;
  /// The result of the last "find" operation.
  std::string last_result;
public:
  /// Adds an environment list variable to the path list.
  void add_env_list(const char* name, char separator) override {

    auto* value = std::getenv(name);
    if (value != nullptr) {
      add_path_list(value, separator);
    }
  }
  /// Adds a list of paths to the program finder.
  /// @param list The list to parse for paths.
  /// @param separator The list separator character.
  void add_path_list(const char* list, char separator) {

    std::string path;

    for (std::size_t i = 0; list[i]; i++) {
      if ((list[i] == '\\') && (list[i + 1] == separator)) {
        // Escaped separator character
        path.push_back(list[++i]);
      } else if ((list[i] == separator) && path.size()) {
        path_list.emplace_back(std::move(path));
      } else {
        path.push_back(list[i]);
      }
    }

    if (path.size()) {
      path_list.emplace_back(std::move(path));
    }
  }
  /// Adds a registry path to the path list.
  void add_registry_path(const RegistrySpecifier& spec) override {

    auto registry = Registry::make();

    auto value = registry->get(spec);

    if (value.valid()) {
      add_registry_value(value);
    }
  }
  /// Searches for a file.
  /// @param name The name of the file to search for.
  /// @returns True if the file was found, false if it was not.
  bool find(const char* basename, const char* ext) override {

    for (const auto& prefix : path_list) {

      auto path = make_path(prefix, basename, ext);

      if (exists(path.c_str())) {
        last_result = path;
        return true;
      }
    }

    return false;
  }
  /// Accesses the result of the last "find" operation.
  const char* get_last_result() const noexcept override {
    return last_result.c_str();
  }
protected:
  /// Indicates whether or not a file exists at a certain path.
  /// @param path The path to open the file at.
  /// @returns True if the file exists, false otherwise.
  bool exists(const char* path) {
    // TODO : On Linux, we could go a step
    // further and check if the file has execute permission.
    std::ifstream file(path);
    return file.good();
  }
  /// Creates a new path.
  /// @param prefix The prefix of the path to make.
  /// @param basename The basename of the path.
  /// @param ext The extension to append to the basename.
  std::string make_path(const std::string& prefix, const char* basename, const char* ext) {

    if (prefix.empty()) {
      return std::string(basename) + ext;
    }

#ifdef _WIN32
    char separator = '\\';
#else
    char separator = '/';
#endif

    std::string output = prefix;

    if (output[output.size() - 1] != separator) {
      output += separator;
    }

    output += basename;
    output += ext;

    return output;
  }
  /// Adds a string from the registry to the path list.
  /// @param value The registry string to add.
  void add_registry_value(const Registry::String& value) {

    std::string path;

    for (std::size_t i = 0; i < value.size(); i++) {
      path.push_back(value[i]);
    }

    path_list.emplace_back(std::move(path));
  }
};

} // namespace

ScopedPtr<ProgramFinder> ProgramFinder::make() {
  return new ProgramFinderImpl();
}

} // namespace sysinfo

} // namespace herald
