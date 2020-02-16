#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectManager;

/// A view of a game project.
class ProjectView {
public:
  /// Creates a new instance of a project view.
  /// @param manager A pointer to the project manager
  /// to get data from and put data to.
  /// @returns A new project view instance.
  static ScopedPtr<ProjectView> make(ProjectManager* manager);
  /// Just a stub.
  virtual ~ProjectView() {}
  /// Shows the project view.
  virtual void show() = 0;
  /// Hides the project view.
  virtual void hide() = 0;
};

} // namespace tk

} // namespace herald
