#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// A view of a game project.
class ProjectView {
public:
  /// Creates a new instance of a project view.
  /// @returns A new project view instance.
  static ScopedPtr<ProjectView> make();
  /// Just a stub.
  virtual ~ProjectView() {}
};

} // namespace tk

} // namespace herald
