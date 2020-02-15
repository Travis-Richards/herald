#include "ProjectView.h"

#include "ScopedPtr.h"

#include <QWidget>

namespace herald {

namespace tk {

namespace {

/// This is the implementation of the project view inteface.
class ProjectViewImpl final : public ProjectView {
  /// A pointer to the root widget.
  ScopedPtr<QWidget> root_widget;
public:
  /// Constructs a new instance of the project view implementation.
  ProjectViewImpl() : root_widget(new QWidget()) { }
};

} // namespace

ScopedPtr<ProjectView> ProjectView::make() {
  return new ProjectViewImpl;
}

} // namespace tk

} // namespace herald
