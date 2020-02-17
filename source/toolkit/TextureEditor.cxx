#include "TextureEditor.h"

#include "ProjectManager.h"
#include "ScopedPtr.h"
#include "TableItemEditor.h"
#include "TextureView.h"

#include <QFileDialog>
#include <QLayout>

namespace herald {

namespace tk {

namespace {

/// Used for editing textures.
/// Displays textures from the texture table
/// and allows for simple effects and transformations.
class TextureEditor final : public TableItemEditor {
  /// A pointer to the project manager to get and put textures to.
  ProjectManager* manager;
  /// The widget displaying the texture.
  ScopedPtr<TextureView> texture_widget;
public:
  /// Constructs a new instance of the texture editor.
  /// @param m A pointer to the project manager.
  /// This is used to load textures and to add
  /// textures to the project.
  TextureEditor(ProjectManager* m) : manager(m) {}
  /// Adds a new texture to the table.
  /// @returns The name to give the new texture.
  QString add() override {

    auto filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open a Texture"));

    return manager->add_texture(filename);
  }
  /// Deletes a texture from the table.
  void del(const QString& name) override {
    manager->delete_texture(name);
  }
  /// Lists the existing textures in the model.
  QStringList list() override {
    return manager->list_textures();
  }
  /// Renames an item from the table.
  /// @param index The index of the item being renamed.
  /// @param name The name given to the item.
  void rename(std::size_t index, const QString& name) override {
    manager->rename_texture(index, name);
  }
  /// Selects the texture to edit.
  /// @param name The name of the selected texture.
  void select(const QString& name) override {
    texture_widget->open_texture(manager->texture_path(name));
  }
  /// Sets up the widget used to view the textures.
  /// @param widget A pointer to the widget to setup.
  void setup_widget(QWidget* parent) override {

    texture_widget = TextureView::make(parent);

    parent->layout()->addWidget(texture_widget->get_widget());
  }
};

} // namespace

ScopedPtr<TableItemEditor> make_texture_editor(ProjectManager* manager) {
  return new TextureEditor(manager);
}

} // namespace tk

} // namespace herald
