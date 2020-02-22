#include <herald/sysinfo/ProgramFinder.h>
#include <herald/sysinfo/Registry.h>

#include <herald/ScopedPtr.h>

#include <iostream>

using namespace herald;
using namespace herald::sysinfo;

namespace {

std::ostream& operator << (std::ostream& output, const Registry::String& str) {
  for (std::size_t i = 0; str[i]; i++) {
    output << str[i];
  }
  return output;
}

/// Queries a value and prints the result.
/// @param key The key to query.
/// @param sub_key The sub key to query.
/// @param value The value to query.
void query_and_print(Registry& reg, const Registry::Specifier& spec) {
  std::cout << reg.get(spec) << std::endl;
}

} // namespace

int main() {

  auto registry = Registry::make();

  query_and_print(*registry, Registry::Specifier(Registry::Key::CurrentUser, "Environment", "Qt5_DIR"));
  query_and_print(*registry, Registry::Specifier(Registry::Key::CurrentUser, "Environment", "Path"));

  auto program_finder = ProgramFinder::make();

  program_finder->add_env_list("PATH");

  program_finder->add_registry_path(Registry::Specifier(Registry::Key::LocalMachine, "JavaSoft", "Java Runtime Environment"));

  if (program_finder->find("java")) {
    std::cerr << "Java found at: " << program_finder->get_last_result() << std::endl;
  } else {
    std::cerr << "Java not found" << std::endl;
  }

  return 0;
}
