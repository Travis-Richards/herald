#include "TextureEditor.h"

#include "ProjectManager.h"
#include "ScopedPtr.h"
#include "TableItemEditor.h"

#include <QFileDialog>
#include <QLabel>
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
  void del(std::size_t) override {
  }
  /// Lists the existing textures in the model.
  QStringList list() override {
    return manager->list_textures();
  }
  /// Sets up the widget used to view the textures.
  /// @param widget A pointer to the widget to setup.
  void setup_widget(QWidget* widget) override {

    auto* layout = widget->layout();

    auto* label = new QLabel(widget);
    label->setBackgroundRole(QPalette::Base);
    label->setSizePolicy(QSizePolicy::Expanding,
                         QSizePolicy::Expanding);
    label->setScaledContents(true);

    layout->addWidget(label);
  }
};

} // namespace

ScopedPtr<TableItemEditor> make_texture_editor(ProjectManager* manager) {
  return new TextureEditor(manager);
}

} // namespace tk

} // namespace herald
