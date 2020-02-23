#include "Manager.h"

#include <herald/ScopedPtr.h>

#include "ProjectManager.h"
#include "ProjectView.h"

namespace herald {

namespace tk {

namespace {

/// The implementation of the manager interface.
class ManagerImpl final : public Manager {
  /// The project manager.
  ScopedPtr<ProjectManager> project_manager;
  /// A view of the game project.
  ScopedPtr<ProjectView> project_view;
public:
  /// Opens a project.
  bool open_project(const QString& game_path) override {

    project_manager = ProjectManager::make();

    if (!project_manager->open(game_path)) {
      return false;
    }

    project_view = ProjectView::make(project_manager.get());

    project_view->show();

    return true;
  }
};

} // namespace

ScopedPtr<Manager> Manager::make() {
  return new ManagerImpl;
}

} // namespace tk

} // namespace herald
