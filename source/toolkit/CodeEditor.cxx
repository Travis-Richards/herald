#include "CodeEditor.h"

#include "Console.h"
#include "GameProcess.h"
#include "Language.h"
#include "ProcessQueue.h"
#include "ProjectManager.h"
#include "SourceFile.h"
#include "SourceTreeModel.h"

#include <herald/ScopedPtr.h>

#include <herald/QtEngine.h>
#include <herald/QtTarget.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSaveFile>
#include <QTabWidget>
#include <QTextStream>
#include <QTreeView>

#include <QCodeEditor>
#include <QJavaHighlighter>
#include <QPythonCompleter>
#include <QPythonHighlighter>

namespace herald {

namespace tk {

namespace {

/// Responsible for editing a single source file.
class OpenedFileEditor final {
  /// The source file content editor.
  QCodeEditor editor;
  /// The source file being edited.
  SourceFile* source_file;
public:
  /// Constructs a new instance of the opened file editor.
  /// @param parent A pointer to the parent widget.
  OpenedFileEditor(SourceFile* source_file_, QWidget* parent)
      : editor(parent), source_file(source_file_) {

    configure_editor();
  }
  /// Connects the modification signal to the tab
  /// widget so that we can notify our user that
  /// the modification has been acknowledged.
  void connect_label_update(QTabWidget* tab_widget) {

    auto mod_functor = [this, tab_widget](bool modified) {

      auto tab_index = tab_widget->indexOf(get_widget());

      const auto* suffix = modified ? " \u2022" : "";

      tab_widget->setTabText(tab_index, source_file->get_name() + suffix);
    };

    QObject::connect(editor.document(), &QTextDocument::modificationChanged, mod_functor);
  }
  /// Accesses the root widget of the editor.
  QWidget* get_widget() {
    return &editor;
  }
  /// Indicates if this editor has a specified source file.
  /// @returns True if it has the source file, false otherwise.
  inline bool contains(const SourceFile* other_source_file) const noexcept {
    return source_file->get_id() == other_source_file->get_id();
  }
  /// Whether or not the code was modified.
  /// @returns True if the code was modified, false otherwise.
  inline bool is_modified() const noexcept {
    return editor.document()->isModified();
  }
  /// Saves the file if the contents were edited.
  /// @returns True on success, false on failure.
  bool save_if_modified() {

    if (!is_modified()) {
      return true;
    }

    QSaveFile save_file(source_file->get_id());

    if (!save_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      return false;
    }

    QTextStream stream(&save_file);

    stream << editor.document()->toPlainText();

    if (!save_file.commit()) {
      return false;
    }

    editor.document()->setModified(false);

    return true;
  }
protected:
  /// Configures the editor for the source file.
  void configure_editor() {

    editor.setWordWrapMode(QTextOption::NoWrap);

    switch (source_file->get_type()) {
      case SourceFileType::Binary:
      case SourceFileType::Unknown:
        break;
      case SourceFileType::Java:
        editor.setHighlighter(new QJavaHighlighter);
        break;
      case SourceFileType::Python:
        editor.setHighlighter(new QPythonHighlighter);
        editor.setCompleter(new QPythonCompleter);
        break;
    }

    if (source_file->get_type() != SourceFileType::Binary) {
      editor.setText(source_file->read_content());
    } else {
      editor.setPlaceholderText("Binary Contents");
      editor.setReadOnly(true);
    }

    editor.document()->setModified(false);
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

    tab_widget.setMovable(true);
    tab_widget.setTabsClosable(true);

    auto close_functor = [this](int index) { close_tab(index); };

    QObject::connect(&tab_widget, &QTabWidget::tabCloseRequested, close_functor);
  }
  /// Accesses the root widget for the file manager.
  QWidget* get_widget() noexcept {
    return &tab_widget;
  }
  /// Opens a new source file.
  /// @param source_file The source file to edit.
  void open(SourceFile* source_file) {

    auto editor = ScopedPtr<OpenedFileEditor>(new OpenedFileEditor(source_file, &tab_widget));

    editor->connect_label_update(&tab_widget);

    auto tab_index = tab_widget.addTab(editor->get_widget(), source_file->get_name());

    tab_widget.setCurrentIndex(tab_index);

    editor_vec.emplace_back(std::move(editor));
  }
  /// Attempts to switch to an existing source file, if it is opened.
  /// @param source_file The source file to switch to.
  /// @returns True if the source file was found, false otherwise.
  bool open_existing(const SourceFile* source_file) {

    for (std::size_t i = 0; i < editor_vec.size(); i++) {
      if (editor_vec[i]->contains(source_file)) {
        tab_widget.setCurrentIndex((int) i);
        return true;
      }
    }

    return false;
  }
  /// Saves all modified documents.
  /// @returns True on success, false on failure.
  bool save_modified() {

    auto success = true;

    for (auto& editor : editor_vec) {
      success &= editor->save_if_modified();
    }

    return success;
  }
protected:
  /// Attempts to close a tab, if it hasn't been modified.
  /// @param index The index of the tab to close.
  /// @returns True if the tab was closed, false otherwise.
  bool close_tab(int index) {

    std::size_t uindex = (std::size_t) index;

    if (uindex >= editor_vec.size()) {
      // Probably won't happen but
      // just to be sure.
      return false;
    }

    if (!editor_vec[uindex]->is_modified()) {
      tab_widget.removeTab(index);
      editor_vec.erase(editor_vec.begin() + uindex);
      return true;
    } else {
      return false;
    }
  }
};

/// A derived tree view for specially handling
/// the source tree directory.
class SourceTreeView final : public QTreeView {
  /// The source tree model
  /// that the source tree view is for.
  SourceTreeModel* model;
  /// The context menu for folders.
  QMenu folder_menu;
  /// The context menu for files.
  QMenu file_menu;
public:
  /// Constructs a new instance of the source tree view.
  /// @param m A pointer to the source tree model.
  /// @param widget A pointer to the parent widget.
  SourceTreeView(SourceTreeModel* m, QWidget* parent) : QTreeView(parent), model(m) {

    setModel(model);
    setRootIndex(model->index(model->rootPath()));

    for (int i = 1; i < model->columnCount(); i++) {
      setColumnHidden(i, true);
    }

    setContextMenuPolicy(Qt::CustomContextMenu);

    add_menu_options();

    connect(this, &SourceTreeView::customContextMenuRequested, this, &SourceTreeView::run_context_menu);
  }
protected:
  /// Adds the options for context menus.
  void add_menu_options() {
    add_file_menu_options();
    add_folder_menu_options();
  }
  /// Creates the file menu options.
  void add_file_menu_options() {

    auto* delete_action = file_menu.addAction(tr("Delete"));

    connect(delete_action, &QAction::triggered, this, &SourceTreeView::delete_file);
  }
  /// Adds the options for folder context menus.
  void add_folder_menu_options() {

    auto* create_file_action = folder_menu.addAction(tr("Create File"));
    auto* create_dir_action  = folder_menu.addAction(tr("Create Folder"));

    connect(create_file_action, &QAction::triggered, this, &SourceTreeView::create_file);
    connect(create_dir_action,  &QAction::triggered, this, &SourceTreeView::create_dir);
  }
  /// Creates a context menu for a certain point on the source tree view.
  /// @param point The point at which to show the menu.
  void run_context_menu(const QPoint& point) {

    auto index = indexAt(point);

    choose_menu(index).exec(viewport()->mapToGlobal(point));
  }
  /// Chooses the menu to be executed, based on an item index.
  /// @param index The index of the item that was clicked when
  /// the menu was requested.
  QMenu& choose_menu(const QModelIndex& index) {
    if (!index.isValid() || model->isDir(index)) {
      return folder_menu;
    } else {
      return file_menu;
    }
  }
  /// Creates a new file.
  void create_file() {

    auto indices = selectedIndexes();

    for (auto index : indices) {
      model->create_source_file(index);
    }
  }
  /// Creates a new directory.
  void create_dir() {

    auto indices = selectedIndexes();

    if (indices.isEmpty()) {
      model->create_dir(model->index(model->rootPath()));
    }

    for (auto index : indices) {
      model->create_dir(index);
    }
  }
  /// Deletes the selected file.
  void delete_file() {

    auto buttons = QMessageBox::Yes | QMessageBox::Cancel;

    auto def_button = QMessageBox::Cancel;

    auto hit_button = QMessageBox::warning(this, tr("Confirm Deletion"), tr("Are you sure you'd like to delete this?"), buttons, def_button);

    if (hit_button == QMessageBox::Yes) {
      delete_file_without_confirmation();
    }
  }
  /// Deletes the selected files without confirmation.
  void delete_file_without_confirmation() {

    auto indices = selectedIndexes();

    for (auto index : indices) {
      model->remove(index);
    }
  }
};

/// The implementation of the code editor interface.
class CodeEditorImpl final : public CodeEditor {
  /// The source code tree model.
  SourceTreeModel* source_tree;
  /// A pointer to the language used by the project.
  Language* language;
  /// The widget containing the code editor contents.
  ScopedPtr<QWidget> root_widget;
  /// A pointer to the buttons widget.
  ScopedPtr<QWidget> buttons_widget;
  /// The button for building the project.
  ScopedPtr<QPushButton> build_button;
  /// The button for running the project.
  ScopedPtr<QPushButton> run_button;
  /// Manages the opened files.
  ScopedPtr<OpenedFileManager> opened_file_manager;
  /// A view of the source code tree.
  ScopedPtr<QTreeView> source_tree_view;
  /// The console to log process output to.
  ScopedPtr<Console> console;
  /// A queue of processes used to build the source files.
  ScopedPtr<ProcessQueue> process_queue;
  /// The window that the game is being rendered to.
  ScopedPtr<QtTarget> game_target;
  /// A pointer to the game engine.
  ScopedPtr<QtEngine> game_engine;
  /// The process running the game.
  /// This pointer may be null when the
  /// game isn't running.
  ScopedPtr<GameProcess> game_process;
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
    : source_tree(nullptr), language(nullptr) {

    source_tree = manager->get_source_tree_model();

    language = manager->get_language();

    root_widget = ScopedPtr<QWidget>(new QWidget(parent));

    QGridLayout* layout = new QGridLayout(root_widget.get());

    // Create  console widget

    console = Console::make(root_widget.get());

    // Create the process queue for
    // building the source files.

    process_queue = ProcessQueue::make(console.get());

    // Create code editor widget

    opened_file_manager = ScopedPtr<OpenedFileManager>(new OpenedFileManager(root_widget.get()));

    source_tree_view = make_source_tree(root_widget.get());

    // Create buttons widget

    buttons_widget = make_buttons_widget(root_widget.get());

    // Add main widgets to layout

    layout->addWidget(buttons_widget.get(),              0,                    0,                button_widget_height, button_widget_width);
    layout->addWidget(source_tree_view.get(),            button_widget_height, 0,                fs_widget_height,     fs_widget_width);
    layout->addWidget(opened_file_manager->get_widget(), button_widget_height, fs_widget_width,  code_editor_height,   code_editor_width);
    layout->addWidget(console->get_widget(),             console_y_offset,     console_x_offset, console_height,       console_width);
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

    auto tree_view = ScopedPtr<SourceTreeView>(new SourceTreeView(source_tree, parent));

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

    build_button = ScopedPtr<QPushButton>(new QPushButton(QObject::tr("Build"), buttons_widget.get()));
    run_button   = ScopedPtr<QPushButton>(new QPushButton(QObject::tr("Run"),   buttons_widget.get()));

    process_queue->enable_on_completion(build_button.get());
    process_queue->enable_on_completion(run_button.get());

    auto* save_button = new QPushButton(QObject::tr("Save"),            buttons_widget.get());

    save_button->setShortcut(Qt::Key_S | Qt::CTRL);
    build_button->setShortcut(Qt::Key_B | Qt::CTRL);
    run_button->setShortcut(Qt::Key_R | Qt::CTRL);

    auto save_functor  = [this](bool) { save(); };
    auto build_functor = [this](bool) { build(); };
    auto run_functor   = [this](bool) { run(); };

    QObject::connect(save_button,            &QPushButton::clicked, save_functor);
    QObject::connect(build_button.get(),     &QPushButton::clicked, build_functor);
    QObject::connect(run_button.get(),       &QPushButton::clicked, run_functor);

    layout->addWidget(save_button);
    layout->addWidget(build_button.get());
    layout->addWidget(run_button.get());

    return buttons_widget;
  }
  /// Opens a source file.
  /// @param index The index of the source file from the file system model.
  /// @returns True on success, false on failure.
  bool open_source_file(const QModelIndex& index) {

    auto source_file = source_tree->open(index);

    if (source_file->get_type() == SourceFileType::Binary) {
      return false;
    }

    if (!opened_file_manager->open_existing(source_file)) {
      opened_file_manager->open(source_file);
    }

    return true;
  }
  /// Builds the project.
  void build() {

    run_button->setEnabled(false);

    build_button->setEnabled(false);

    console->clear();

    console->println("Starting build", Console::Tag::Info);

    language->build(*process_queue.get(), *source_tree);

    process_queue->start_all();
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
  /// Saves the source code that was modified.
  void save() {
    opened_file_manager->save_modified();
  };
};

} // namespace

ScopedPtr<CodeEditor> CodeEditor::make(ProjectManager* manager, QWidget* parent) {
  return new CodeEditorImpl(manager, parent);
}

} // namespace tk

} // namespace herald
