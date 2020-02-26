#include "TextureEditor.h"

#include <herald/ScopedPtr.h>

#include "Project.h"
#include "TableButton.h"
#include "TableEditor.h"
#include "TableModel.h"
#include "TextureView.h"

#include <QFileDialog>
#include <QLayout>
#include <QSplitter>

namespace herald {

namespace tk {

namespace {

/// This is a wrapper around the texture
/// table to act as a table model for the table editor.
class TextureTableModel final : public TableModel {
  /// The project model containing the textures.
  Project* project;
public:
  /// Constructs a new instance of the texture table model.
  /// @param m The project model containing the textures.
  TextureTableModel(Project* m) : project(m) {}
  /// Adds a texture to the model.
  /// @param path The path of the texture to add.
  void add_texture(const QString& path) {

    auto table_size = project->access_texture_table()->size();

    auto last_index = index(table_size, 0);

    beginInsertRows(last_index, table_size, table_size + 1);

    project->modify_texture_table()->add(path);

    endInsertRows();
  }
  /// Accesses the data from a certain texture.
  /// @param index The index of the texture to get the data of.
  /// @returns The data for the specified texture.
  QByteArray get_data(std::size_t index) const {
    return project->access_texture_table()->get_data(index);
  }
protected:
  /// Removes a texture from the model.
  /// @param index The index of the texture to remove.
  /// @returns True on success, false on failure.
  bool remove(std::size_t index) override {
    return project->modify_texture_table()->remove(index);
  }
  /// Gets the name of a texture.
  QString get_name(std::size_t index) const override {
    return project->access_texture_table()->get_name(index);
  }
  /// Gets the number of textures in the texture table.
  std::size_t get_size() const override {
    return project->access_texture_table()->size();
  }
  /// Sets the name of an texture.
  /// @param index The index of the texture to set.
  /// @param name The name to give the texture.
  /// @returns True on success, false on failure.
  bool rename(std::size_t index, const QString& name) override {
    return project->modify_texture_table()->rename(index, name);
  }
};

/// Used for editing textures.
/// Displays textures from the texture table
/// and allows for simple effects and transformations.
class TextureEditorImpl final : public TextureEditor {
  /// Identifies the "Import" button.
  static constexpr std::size_t import_button_id = 0;
  /// The root widget.
  ScopedPtr<QSplitter> root_widget;
  /// The model for the texture table.
  ScopedPtr<TextureTableModel> table_model;
  /// The table editor widget.
  ScopedPtr<TableEditor> table_editor;
  /// The widget displaying the texture.
  ScopedPtr<TextureView> texture_widget;
public:
  /// Constructs a new texture editor instance.
  /// @param project The project data to get the textures from.
  /// @param parent A pointer to the parent widget.
  TextureEditorImpl(Project* project, QWidget* parent) {

    root_widget = ScopedPtr<QSplitter>(new QSplitter(Qt::Horizontal, parent));

    table_model = ScopedPtr<TextureTableModel>(new TextureTableModel(project));

    table_editor = TableEditor::make(table_model.get(), root_widget.get());
    table_editor->add_button(import_button_id, QObject::tr("Import"));

    texture_widget = TextureView::make(root_widget.get());

    root_widget->addWidget(table_editor->get_widget());
    root_widget->addWidget(texture_widget->get_widget());
    root_widget->setSizes(QList<int>({ INT_MAX, INT_MAX }));

    auto button_functor = [this](const TableButtonID& id) { handle_button(id); };

    auto display_functor = [this](std::size_t index) { display(index); };

    QObject::connect(table_editor.get(), &TableEditor::button_clicked, button_functor);
    QObject::connect(table_editor.get(), &TableEditor::selected, display_functor);
  }
  /// Accesses a pointer to the root widget.
  QWidget* get_widget() noexcept override {
    return root_widget.get();
  }
protected:
  /// Displays a texture from the model.
  /// @param index The index of the texture to display.
  void display(std::size_t index) {
    texture_widget->open_texture(table_model->get_data(index));
  }
  /// Handles a button being clicked.
  /// @param name The name of the button that was clicked.
  /// @returns True on success, false on failure.
  bool handle_button(const TableButtonID& button_id) {

    if (button_id.is_predefined()) {
      if (button_id.has_id(TableEditor::remove_button_id)) {
        texture_widget->clear();
      }
    } else if (button_id.has_id(import_button_id)) {
      return add_texture();
    }

    return true;
  }
  /// Adds a new texture to the table.
  /// @returns True on success, false on failure.
  bool add_texture() {

    auto filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open a Texture"));

    table_model->add_texture(filename);

    return true;
  }
};

} // namespace

ScopedPtr<TextureEditor> TextureEditor::make(Project* project, QWidget* parent) {
  return new TextureEditorImpl(project, parent);
}

} // namespace tk

} // namespace herald
