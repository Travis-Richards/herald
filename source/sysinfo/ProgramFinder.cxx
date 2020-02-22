#include <herald/sysinfo/ProgramFinder.h>

#include <herald/ScopedPtr.h>

namespace herald {

namespace sysinfo {

namespace {

/// The implementation of the program finder interface.
class ProgramFinderImpl final : public ProgramFinder {
public:
};

} // namespace

ScopedPtr<ProgramFinder> ProgramFinder::make() {
  return new ProgramFinderImpl();
}

} // namespace sysinfo

} // namespace herald
