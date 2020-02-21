#include "ProjectView.h"

#include <herald/ScopedPtr.h>

#include "ActionEditor.h"
#include "AnimationEditor.h"
#include "CodeEditor.h"
#include "GameInfo.h"
#include "ProjectManager.h"
#include "RoomEditor.h"
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
  /// A pointer to the code editor tab.
  ScopedPtr<CodeEditor> code_editor;
  /// A pointer to the room editor.
  ScopedPtr<TableEditor> room_editor;
  /// A pointer to the action editor.
  ScopedPtr<TableEditor> action_editor;
  /// A pointer to the animation editor.
  ScopedPtr<TableEditor> animation_editor;
  /// A pointer to the texture editor.
  ScopedPtr<TableEditor> texture_editor;
public:
  /// Constructs a new instance of the project view implementation.
  /// @param manager A pointer to the project manager.
  ProjectViewImpl(ProjectManager* manager) {

    main_window = ScopedPtr<QMainWindow>(new QMainWindow());

    tab_widget = ScopedPtr<QTabWidget>(new QTabWidget(main_window.get()));

    code_editor = CodeEditor::make(manager, tab_widget.get());

    tab_widget->addTab(code_editor->get_widget(), QObject::tr("Code"));
    tab_widget->addTab(make_rooms_tab(manager, tab_widget.get()),      QObject::tr("Rooms"));
    tab_widget->addTab(make_actions_tab(manager, tab_widget.get()),    QObject::tr("Actions"));
    tab_widget->addTab(make_animations_tab(manager, tab_widget.get()), QObject::tr("Animations"));
    tab_widget->addTab(make_textures_tab(manager, tab_widget.get()),   QObject::tr("Textures"));

    main_window->setCentralWidget(tab_widget.get());
    main_window->setMinimumSize(640, 480);
  }
  /// Hides the window.
  void hide() override {
    main_window->hide();
  }
  /// Shows the window.
  void show() override {
    main_window->show();
  }
protected:
  /// Creates the rooms tab.
  /// @param manager A pointer to the project manager.
  /// @param parent A pointer to the parent widget.
  QWidget* make_rooms_tab(ProjectManager* manager, QWidget* parent) {
    room_editor = TableEditor::make(make_room_editor(manager), parent);
    return room_editor->get_widget();
  }
  /// Creates the actions tab.
  /// @param manager A pointer to the project manager.
  /// @param parent A pointer to the parent widget.
  QWidget* make_actions_tab(ProjectManager* manager, QWidget* parent) {
    action_editor = TableEditor::make(make_action_editor(manager), parent);
    return action_editor->get_widget();
  }
  /// Creates the animations tab.
  /// @param parent A pointer to the parent widget.
  QWidget* make_animations_tab(ProjectManager* manager, QWidget* parent) {
    animation_editor = TableEditor::make(make_animation_editor(manager), parent);
    return animation_editor->get_widget();
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
