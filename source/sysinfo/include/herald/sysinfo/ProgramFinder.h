#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace sysinfo {

/// Used for locating programs on the system.
class ProgramFinder {
public:
  /// Creates a new program finder instance.
  /// @returns A new program finder instance.
  static ScopedPtr<ProgramFinder> make();
  /// Just a stub.
  virtual ~ProgramFinder() {}
};

} // namespace sysinfo

} // namespace herald
