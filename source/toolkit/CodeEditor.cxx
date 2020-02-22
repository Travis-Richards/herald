#include "CodeEditor.h"

#include "Console.h"
#include "Language.h"
#include "ProcessQueue.h"
#include "ProjectManager.h"
#include "SourceFile.h"
#include "SourceManager.h"

#include <herald/ScopedPtr.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeView>

#include <QCodeEditor>
#include <QJavaHighlighter>
#include <QPythonHighlighter>

namespace herald {

namespace tk {

namespace {

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
  /// A pointer to the code editing widget.
  ScopedPtr<QCodeEditor> code_editor;
  /// A view of the source code tree.
  ScopedPtr<QTreeView> source_tree_view;
  /// The console for program output.
  ScopedPtr<Console> console;
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

    code_editor = ScopedPtr<QCodeEditor>(new QCodeEditor(root_widget.get()));
    code_editor->setWordWrapMode(QTextOption::NoWrap);
    code_editor->setReadOnly(true);

    source_tree_view = make_source_tree(root_widget.get());

    // Create buttons widget

    buttons_widget = make_buttons_widget(root_widget.get());

    // Create  console widget

    console = Console::make(root_widget.get());

    // Add main widgets to layout

    layout->addWidget(buttons_widget.get(),   0,                    0,                button_widget_height, button_widget_width);
    layout->addWidget(source_tree_view.get(), button_widget_height, 0,                fs_widget_height,     fs_widget_width);
    layout->addWidget(code_editor.get(),      button_widget_height, fs_widget_width,  code_editor_height,   code_editor_width);
    layout->addWidget(console->get_widget(),  console_y_offset,     console_x_offset, console_height,       console_width);

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

    auto build_functor = [this](bool) {
      language->build(*process_queue.get(), *source_manager);
    };

    QObject::connect(new_source_file_button, &QPushButton::clicked, new_source_functor);
    QObject::connect(save_button,            &QPushButton::clicked, save_functor);
    QObject::connect(build_button,           &QPushButton::clicked, build_functor);

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

    code_editor->setDocument(source_file->get_code());

    auto read_only = false;

    switch (source_file->get_type()) {
      case SourceFileType::Invalid:
        read_only = true;
        break;
      case SourceFileType::Java:
        code_editor->setHighlighter(new QJavaHighlighter);
        break;
      case SourceFileType::Python:
        code_editor->setHighlighter(new QPythonHighlighter);
        break;
    }

    code_editor->setReadOnly(read_only);

    return true;
  }
};

} // namespace

ScopedPtr<CodeEditor> CodeEditor::make(ProjectManager* manager, QWidget* parent) {
  return new CodeEditorImpl(manager, parent);
}

} // namespace tk

} // namespace herald
