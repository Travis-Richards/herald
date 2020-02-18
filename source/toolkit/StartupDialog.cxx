#include "StartupDialog.h"

#include <herald/ScopedPtr.h>

#include "GameInfo.h"
#include "GameList.h"
#include "Manager.h"

#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QTableWidget>
#include <QWidget>

#include <random>

namespace herald {

namespace tk {

namespace {

/// Used for creating a project
/// if the user hits the "new" button.
class ProjectCreator final : public QObject {
  /// The API to assign the project.
  QString api;
  /// The name of the author making the project.
  QString author;
  /// The location to create the game at.
  QString location;
  /// The title of the game.
  QString title;
public:
  /// Constructs a new instance of the project creator.
  /// @param parent A pointer to the parent object.
  ProjectCreator(Manager*, QObject* parent) : QObject(parent) {}
  /// Gets the path to the game.
  QString get_game_path() {
    return QDir::cleanPath(location + QDir::separator() + title);
  }
  /// Indicates whether or not the creator
  /// is ready to make a project. Internally,
  /// it checks to make sure that a title is
  /// present and the location is valid.
  /// @returns True if it's ready, false otherwise.
  bool ready() const noexcept {

    if (title.isEmpty() || location.isEmpty()) {
      return false;
    } else if (title.contains('\\') || title.contains('/')) {
      return false;
    }

    QFileInfo location_info(location);

    if (!location_info.exists()) {
      return false;
    } else if (!location_info.isDir()) {
      return false;
    }

    QFileInfo dir_info(QDir::cleanPath(location + QDir::separator() + title));

    // Make sure the game directory
    // doesn't already exist so we
    // don't accidently overwrite something.
    return !dir_info.exists();
  }
public slots:
  /// Creates the project.
  /// @returns True on success, false on failure.
  bool create() {

    add_to_game_list();

    return make_info_file();
  }
  /// Chooses the API to make the project in.
  /// @param api_ The choosen API name.
  void choose_api(const QString& api_) {
    api = api_;
  }
  /// Chooses the author name for a new game.
  /// @param author_ The author name for a new game.
  void choose_author(const QString& author_) {
    author = author_;
  }
  /// Chooses the location to open the game at.
  void choose_location(const QString& location_) {
    location = location_;
  }
  /// Chooses the title to give the game
  /// that may be opened.
  void choose_title(const QString& title_) {
    title = title_;
  }
protected:
  /// Adds the game location to the game list.
  void add_to_game_list() {

    QSettings settings;

    auto game_list = settings.value("gamelist").toStringList();

    game_list << QDir::cleanPath(location + QDir::separator() + title);

    settings.setValue("gamelist", game_list);
  }
  /// Creates the information file used to open the game.
  /// @returns True on success, false on failure.
  bool make_info_file() {

    QDir game_dir(location);

    if (!game_dir.mkdir(title)) {
      return false;
    } else if (!game_dir.cd(title)) {
      return false;
    }

    QJsonObject json_root;

    json_root.insert("title", title);
    json_root.insert("author", author);
    json_root.insert("api", api);

    QJsonDocument json_doc(json_root);

    QFile file(game_dir.absoluteFilePath("info.json"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      return false;
    }

    file.write(json_doc.toJson());

    return true;
  }
  /// Converts the game title into a
  /// name that can be put onto a directory.
  /// @returns The translated filename, with
  /// every '/' or '\' character replaced with
  /// a space character.
  QString title_to_dirname() const {
    QString out;
    for (auto c : title) {
      if ((c == '\\') || (c == '/')) {
        out += ' ';
      } else {
        out += c;
      }
    }
    return out;
  }
};

/// An implementation of the startup dialog interface.
class StartupDialogImpl final : public StartupDialog {
  /// A pointer to the application manager.
  Manager* manager;
  /// The root widget displaying the dialog.
  ScopedPtr<QWidget> root_widget;
  /// Used for creating projects.
  ScopedPtr<ProjectCreator> project_creator;
  /// The list of existing game projects.
  ScopedPtr<GameList> game_list;
  /// The button to make a new project.
  QPushButton* new_button;
  /// The button to open an existing project.
  QPushButton* open_button;
  /// The last selected game.
  QString selected_game;
public:
  /// Constructs a new instance of the dialog.
  StartupDialogImpl(Manager* m)
    : manager(m),
      root_widget(new QWidget()),
      project_creator(new ProjectCreator(m, root_widget.get())),
      game_list(GameList::open(QSettings())) {

    setup_root_widget();
  }
  /// Shows the widget.
  void show() override {
    root_widget->show();
  }
protected:
  /// Sets up the root widget.
  void setup_root_widget() {

    root_widget->setWindowTitle(QObject::tr("Open a Project"));

    root_widget->setMinimumSize(QSize(640, 480));

    root_widget->setLayout(new QHBoxLayout);

    setup_open_project_widget();
    setup_make_project_widget();
  }
  /// Sets up the widget that opens an existing project.
  void setup_open_project_widget() {

    auto* widget = new QWidget(root_widget.get());

    auto* layout = new QVBoxLayout(widget);

    layout->addWidget(make_existing_project_table(widget));

    layout->addWidget(make_open_existing_button(widget));

    root_widget->layout()->addWidget(widget);
  }
  /// Creates the table containing the existing projects.
  /// @param parent A pointer to the parent widget to assign.
  /// @returns A new table widget instance.
  QTableWidget* make_existing_project_table(QWidget* parent) {

    QStringList headers;
    headers << QObject::tr("Title");
    headers << QObject::tr("Location");

    auto* table = new QTableWidget(parent);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto game_selector = [this](int row, int) {
      select_game(game_list->at((std::size_t) row)->path());
    };

    auto game_opener = [this](int row, int) {

      root_widget->hide();

      manager->open_project(game_list->at((std::size_t) row)->path());
    };

    QObject::connect(table, &QTableWidget::cellClicked, game_selector);

    QObject::connect(table, &QTableWidget::cellDoubleClicked, game_opener);

    game_list->fill_table(table);

    return table;
  }
  /// Creates a button for opening an existing project.
  /// @param parent A pointer to the parent object.
  /// @returns A new push button instance.
  QPushButton* make_open_existing_button(QWidget* parent) {

    open_button = new QPushButton(QObject::tr("Open Existing"), parent);

    open_button->setEnabled(false);

    auto open_functor = [this](bool) {

      root_widget->hide();

      manager->open_project(selected_game);
    };

    QObject::connect(open_button, &QPushButton::clicked, open_functor);

    return open_button;
  }
  /// Sets up the widget that creates a new project.
  void setup_make_project_widget() {

    auto* widget = new QWidget(root_widget.get());

    auto* layout = new QFormLayout(widget);

    layout->addRow(QObject::tr("Title"), make_title_edit_box(widget));

    layout->addRow(QObject::tr("Author"), make_author_edit_box(widget));

    layout->addRow(QObject::tr("API"), make_api_combo_box(widget));

    layout->addRow(QObject::tr("Location"), make_location_widget(widget));

    layout->addWidget(make_new_button(widget));

    root_widget->layout()->addWidget(widget);
  }
  /// Creates the combo box for the API selection.
  /// @param parent A pointer to the parent widget.
  /// @returns A pointer to the combo box.
  QComboBox* make_api_combo_box(QWidget* parent) {

    auto* combo_box = new QComboBox(parent);

    QObject::connect(combo_box, &QComboBox::currentTextChanged, project_creator.get(), &ProjectCreator::choose_api);

    combo_box->addItem(QObject::tr("Java"));
    combo_box->addItem(QObject::tr("Python"));
    combo_box->addItem(QObject::tr("Executable"));

    return combo_box;
  }
  /// Creates a line edit box for the game author.
  /// @param parent A pointer to the parent widget.
  /// @returns A pointer to the author edit box.
  QLineEdit* make_author_edit_box(QWidget* parent) {

    auto* line_edit = new QLineEdit(parent);

    QObject::connect(line_edit, &QLineEdit::textChanged, project_creator.get(), &ProjectCreator::choose_author);

    return line_edit;
  }
  /// Creates a line edit box for the game title.
  /// @param parent A pointer to the parent widget.
  /// @returns A new line edit box for the game title.
  QLineEdit* make_title_edit_box(QWidget* parent) {

    auto* line_edit = new QLineEdit(parent);

    line_edit->setPlaceholderText(make_random_title());

    auto input_validator = [this](const QString& title) {

      project_creator->choose_title(title);

      new_button->setEnabled(project_creator->ready());
    };

    QObject::connect(line_edit, &QLineEdit::textEdited, input_validator);

    return line_edit;
  }
  /// Creates the widget used to choose the new project's location.
  /// @param parent A pointer to the parent widget.
  /// @returns A pointer to the location widget.
  QWidget* make_location_widget(QWidget* parent) {

    auto* widget = new QWidget(parent);

    auto* layout = new QHBoxLayout(widget);

    auto* line_edit = new QLineEdit(widget);

    auto* browse_button = new QPushButton(QObject::tr("Browse"));

    auto* browse_dialog = new QFileDialog(parent, QObject::tr("Choose Game Directory"));
    browse_dialog->setDirectory(QDir::home());
    browse_dialog->setFileMode(QFileDialog::DirectoryOnly);

    auto location_selector = [line_edit](const QString& path) {
      line_edit->setText(path);
    };

    auto input_validator = [this](const QString& location) {

      project_creator->choose_location(location);

      new_button->setEnabled(project_creator->ready());
    };

    QObject::connect(line_edit, &QLineEdit::textChanged, input_validator);

    QObject::connect(browse_button, &QPushButton::clicked, browse_dialog, &QFileDialog::show);

    QObject::connect(browse_dialog, &QFileDialog::fileSelected, location_selector);

    layout->addWidget(line_edit);
    layout->addWidget(browse_button);

    return widget;
  }
  /// Creates the button used to make a new project.
  /// @param parent A pointer to the parent widget.
  /// @returns A push button to make new projects with.
  QPushButton* make_new_button(QWidget* parent) {

    new_button = new QPushButton(QObject::tr("New"), parent);

    new_button->setEnabled(false);

    auto open_functor = [this](bool) {

      root_widget->hide();

      project_creator->create();

      manager->open_project(project_creator->get_game_path());
    };

    QObject::connect(new_button, &QPushButton::clicked, open_functor);

    return new_button;
  }
  /// Randomly selects from a list of game titles.
  /// @returns A random game title.
  QString make_random_title() {

    QStringList titles;
    titles << "Combat Cavern";
    titles << "Immortal Tombs";
    titles << "Sword and Symphony";
    titles << "Deltaphobia";
    titles << "Strike of Salvation";
    titles << "Antimania";
    titles << "Infinite Conflict";
    titles << "Revolt of Logic";
    titles << "Struggle of War";
    titles << "Chase of Power";
    titles << "Invasion of Truth";
    titles << "Forgotten Project";
    titles << "Crush of Assault";
    titles << "Mass Siren";
    titles << "Lunar Evolution";
    titles << "Maze and Magic";
    titles << "Guard of Immunity";

    std::random_device dev;

    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, titles.size() - 1);

    return QString("How about: ") + titles[dist(rng)];
  }
protected slots:
  /// Browses for a location to open a project at.
  void browse(bool) {
    QFileDialog::getExistingDirectory(root_widget.get(), QObject::tr("Choose Game Location"));
  }
  /// Selects a game from the table.
  void select_game(const QString& path) {

    selected_game = path;

    open_button->setEnabled(!selected_game.isEmpty());
  }
};

} // namespace

ScopedPtr<StartupDialog> StartupDialog::make(Manager* manager) {
  return new StartupDialogImpl(manager);
}

} // namespace tk

} // namespace herald
