#include "ProjectView.h"

#include "ScopedPtr.h"
#include "TableEditor.h"
#include "TableItemEditor.h"
#include "TextureEditor.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

namespace herald {

namespace tk {

namespace {

/// This is the implementation of the project view inteface.
class ProjectViewImpl final : public ProjectView {
  /// A pointer to the main window.
  ScopedPtr<QMainWindow> main_window;
  /// A pointer to the widget containing the tabs.
  ScopedPtr<QTabWidget> tab_widget;
  /// A pointer to the texture editor.
  ScopedPtr<TableEditor> texture_editor;
public:
  /// Constructs a new instance of the project view implementation.
  /// @param manager A pointer to the project manager.
  ProjectViewImpl(ProjectManager* manager) {

    main_window = ScopedPtr<QMainWindow>(new QMainWindow());

    tab_widget = ScopedPtr<QTabWidget>(new QTabWidget(main_window.get()));
    tab_widget->addTab(make_rooms_tab(tab_widget.get()),      QObject::tr("Rooms"));
    tab_widget->addTab(make_actions_tab(tab_widget.get()),    QObject::tr("Actions"));
    tab_widget->addTab(make_animations_tab(tab_widget.get()), QObject::tr("Animations"));

    tab_widget->addTab(make_textures_tab(manager, tab_widget.get()),   QObject::tr("Textures"));

    main_window->setCentralWidget(tab_widget.get());
    main_window->setMinimumSize(640, 480);
  }
  /// Shows the window.
  void show() override {
    main_window->show();
  }
  /// Hides the window.
  void hide() override {
    main_window->hide();
  }
protected:
  /// Creates the rooms tab.
  /// @param parent A pointer to the parent widget.
  QWidget* make_rooms_tab(QWidget* parent) {
    return new QWidget(parent);
  }
  /// Creates the actions tab.
  /// @param parent A pointer to the parent widget.
  QWidget* make_actions_tab(QWidget* parent) {
    return new QWidget(parent);
  }
  /// Creates the animations tab.
  /// @param parent A pointer to the parent widget.
  QWidget* make_animations_tab(QWidget* parent) {
    return new QWidget(parent);
  }
  /// Creates the textures tab.
  /// @param manager A pointer to the project manager
  /// used to add and remove textures from the project.
  /// @param parent A pointer to the parent widget.
  QWidget* make_textures_tab(ProjectManager* manager, QWidget* parent) {

    texture_editor = TableEditor::make(make_texture_editor(manager), parent);

    return texture_editor->get_widget();
  }
};

} // namespace

ScopedPtr<ProjectView> ProjectView::make(ProjectManager* manager) {
  return new ProjectViewImpl(manager);
}

} // namespace tk

} // namespace herald
