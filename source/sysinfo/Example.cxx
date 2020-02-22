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

  return 0;
}
