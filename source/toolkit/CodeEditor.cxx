#include "CodeEditor.h"

#include "Console.h"
#include "GameProcess.h"
#include "Language.h"
#include "ProcessQueue.h"
#include "ProjectManager.h"
#include "SourceFile.h"
#include "SourceManager.h"

#include <herald/ScopedPtr.h>

#include <herald/QtEngine.h>
#include <herald/QtTarget.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QTreeView>

#include <QCodeEditor>
#include <QJavaHighlighter>
#include <QPythonHighlighter>

namespace herald {

namespace tk {

namespace {

/// Responsible for editing a single source file.
class OpenedFileEditor final {
  /// The source file content editor.
  QCodeEditor editor;
public:
  /// Constructs a new instance of the opened file editor.
  /// @param parent A pointer to the parent widget.
  OpenedFileEditor(QWidget* parent) : editor(parent) {
    editor.setWordWrapMode(QTextOption::NoWrap);
  }
  /// Accesses the root widget of the editor.
  QWidget* get_widget() {
    return &editor;
  }
};

/// Manages opened source files.
class OpenedFileManager final {
  /// The widget containing the opened file tabs.
  QTabWidget tab_widget;
  /// The container of opened file editors.
  std::vector<ScopedPtr<OpenedFileEditor>> editor_vec;
public:
  /// Constructs a new instance of the opened file manager.
  /// @param parent A pointer to the parent widget.
  OpenedFileManager(QWidget* parent) : tab_widget(parent) {

  }
  /// Accesses the root widget for the file manager.
  QWidget* get_widget() noexcept {
    return &tab_widget;
  }
  /// Opens a new source file.
  /// @param source_file The source file to edit.
  void open(const SourceFile& source_file) {

    (void)source_file;

    auto editor = ScopedPtr<OpenedFileEditor>(new OpenedFileEditor(&tab_widget));

    tab_widget.addTab(editor->get_widget(), "File");

    editor_vec.emplace_back(std::move(editor));
  }
};

/// The implementation of the code editor interface.
class CodeEditorImpl final : public CodeEditor {
  /// The source code manager.
  SourceManager* source_manager;
  /// A pointer to the language used by the project.
  Language* language;
  /// The widget containing the code editor contents.
  ScopedPtr<QWidget> root_widget;
  /// A pointer to the buttons widget.
  ScopedPtr<QWidget> buttons_widget;
  /// Manages the opened files.
  ScopedPtr<OpenedFileManager> opened_file_manager;
  /// A view of the source code tree.
  ScopedPtr<QTreeView> source_tree_view;
  /// The console for program output.
  ScopedPtr<Console> console;
  /// The window that the game is being rendered to.
  ScopedPtr<QtTarget> game_target;
  /// A pointer to the game engine.
  ScopedPtr<QtEngine> game_engine;
  /// The process running the game.
  /// This pointer may be null when the
  /// game isn't running.
  ScopedPtr<GameProcess> game_process;
  /// A queue of processes used to build the source files.
  ScopedPtr<ProcessQueue> process_queue;
  /// The width of the grid layout.
  static constexpr int grid_width = 10;
  /// The height of the grid layout.
  static constexpr int grid_height = 20;
  /// The width, in terms of grid units, of the button widget.
  static constexpr int button_widget_width = grid_width;
  /// The height, in terms of grid units, of the button widget.
  static constexpr int button_widget_height = 1;
  /// The width, in terms of grid units, of the file system widget.
  static constexpr int fs_widget_width = 2;
  /// The height, in terms of grid units, of the file system widget.
  static constexpr int fs_widget_height = grid_height - button_widget_height;
  /// The width, in terms of grid units, of the code editor.
  static constexpr int code_editor_width = grid_width - fs_widget_width;
  /// The height, in terms of grid units, of the code editor.
  static constexpr int code_editor_height = 17;
  /// The Y offset of the console.
  static constexpr int console_y_offset = button_widget_height + code_editor_height;
  /// The X offset of the console.
  static constexpr int console_x_offset = fs_widget_width;
  /// The height of the console, in grid units.
  static constexpr int console_height = grid_height - (button_widget_height + code_editor_height);
  /// The width of the console, in grid units.
  static constexpr int console_width = grid_width - fs_widget_width;
public:
  /// Constructs a new instance of the code editor.
  /// @param manager A pointer to the project manager.
  /// @param parent A pointer to the parent widget.
  CodeEditorImpl(ProjectManager* manager, QWidget* parent)
    : source_manager(nullptr), language(nullptr) {

    source_manager = manager->get_source_manager();

    language = manager->get_language();

    root_widget = ScopedPtr<QWidget>(new QWidget(parent));

    QGridLayout* layout = new QGridLayout(root_widget.get());

    // Create code editor widget

    opened_file_manager = ScopedPtr<OpenedFileManager>(new OpenedFileManager(root_widget.get()));

    source_tree_view = make_source_tree(root_widget.get());

    // Create buttons widget

    buttons_widget = make_buttons_widget(root_widget.get());

    // Create  console widget

    console = Console::make(root_widget.get());

    // Add main widgets to layout

    layout->addWidget(buttons_widget.get(),              0,                    0,                button_widget_height, button_widget_width);
    layout->addWidget(source_tree_view.get(),            button_widget_height, 0,                fs_widget_height,     fs_widget_width);
    layout->addWidget(opened_file_manager->get_widget(), button_widget_height, fs_widget_width,  code_editor_height,   code_editor_width);
    layout->addWidget(console->get_widget(),             console_y_offset,     console_x_offset, console_height,       console_width);

    // Create the process queue for
    // building the source files.

    process_queue = ProcessQueue::make(console.get());
  }
  /// Accesses a pointer to the code editing widget.
  QWidget* get_widget() noexcept override {
    return root_widget.get();
  }
protected:
  /// Creates the source tree view.
  /// @param parent A pointer to the parent widget.
  /// @returns A new source tree view.
  ScopedPtr<QTreeView> make_source_tree(QWidget* parent) {

    auto* source_model = source_manager->get_model();

    auto tree_view = ScopedPtr<QTreeView>(new QTreeView(parent));
    tree_view->setModel(source_model);
    tree_view->setRootIndex(source_manager->get_root());

    for (int i = 1; i < source_model->columnCount(); i++) {
      tree_view->setColumnHidden(i, true);
    }

    auto open_functor = [this](const QModelIndex& index) {
      open_source_file(index);
    };

    QObject::connect(tree_view.get(), &QTreeView::clicked, open_functor);

    return tree_view;
  }
  /// Creates the widget containing the buttons for the code editor.
  /// @param parent A pointer to the parent widget.
  /// @returns The widget containing the buttons.
  ScopedPtr<QWidget> make_buttons_widget(QWidget* parent) {

    ScopedPtr<QWidget> buttons_widget = new QWidget(parent);

    QHBoxLayout* layout = new QHBoxLayout(buttons_widget.get());

    auto* new_source_file_button = new QPushButton(QObject::tr("New Source File"), buttons_widget.get());
    auto* new_directory          = new QPushButton(QObject::tr("New Directory"),   buttons_widget.get());
    auto* save_button            = new QPushButton(QObject::tr("Save"),            buttons_widget.get());
    auto* build_button           = new QPushButton(QObject::tr("Build"),           buttons_widget.get());
    auto* run_button             = new QPushButton(QObject::tr("Run"),             buttons_widget.get());

    auto new_source_functor = [this](bool) {
      source_manager->create_source_file(language->default_extension());
    };

    auto save_functor = [this](bool) {
      source_manager->save_modified();
    };

    auto build_functor = [this](bool) { build(); };

    auto run_functor = [this](bool) { run(); };

    QObject::connect(new_source_file_button, &QPushButton::clicked, new_source_functor);
    QObject::connect(save_button,            &QPushButton::clicked, save_functor);
    QObject::connect(build_button,           &QPushButton::clicked, build_functor);
    QObject::connect(run_button,             &QPushButton::clicked, run_functor);

    layout->addWidget(new_source_file_button);
    layout->addWidget(new_directory);
    layout->addWidget(save_button);
    layout->addWidget(build_button);
    layout->addWidget(run_button);

    return buttons_widget;
  }
  /// Opens a source file.
  /// @param index The index of the source file from the file system model.
  /// @returns True on success, false on failure.
  bool open_source_file(const QModelIndex& index) {

    auto source_file = source_manager->open(index);

    opened_file_manager->open(*source_file);

    return true;
  }
  /// Builds the project.
  void build() {

    console->clear();

    language->build(*process_queue.get(), *source_manager);
  }
  /// Runs the game.
  void run() {

    console->clear();

    if (game_process) {
      game_process->waitOrKill();
    }

    if (!game_target) {
      game_target = QtTarget::make(nullptr);
    }

    if (!game_engine) {
      game_engine = QtEngine::make(game_target.get());
    }

    game_target->show();
  }
};

} // namespace

ScopedPtr<CodeEditor> CodeEditor::make(ProjectManager* manager, QWidget* parent) {
  return new CodeEditorImpl(manager, parent);
}

} // namespace tk

} // namespace herald
