#include "PathSetting.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

namespace {

/// Allows the user to set a path.
class PathSettingImpl final : public PathSetting {
  /// The path browsing button.
  QPushButton* browse_button;
  /// The path text editor.
  QLineEdit* line_edit;
  /// The dialog for choosing the path.
  QFileDialog* file_dialog;
public:
  /// Constructs a new path setting.
  /// @param parent A pointer to the parent widget.
  PathSettingImpl(QWidget* parent): PathSetting(parent) {

    file_dialog = new QFileDialog(this);

    file_dialog->setFilter(QDir::Executable | QDir::Files);

    browse_button = new QPushButton(tr("Browse"), this);

    line_edit = new QLineEdit(this);

    auto* layout = new QHBoxLayout(this);

    connect(browse_button, &QPushButton::clicked,       this, &PathSettingImpl::browse);
    connect(line_edit,     &QLineEdit::editingFinished, this, &PathSettingImpl::update_path_from_line_edit);
    connect(file_dialog,   &QFileDialog::accepted,      this, &PathSettingImpl::update_path_from_dialog);

    layout->addWidget(line_edit);
    layout->addWidget(browse_button);
  }
  /// Assigns the path held by the setting.
  void set_path(const QString& path) override {
    line_edit->setText(path);
  }
protected:
  /// Browses for a path.
  void browse() {
    file_dialog->open();
  }
  /// Updates the path from the dialog box.
  void update_path_from_dialog() {

    auto file_list = file_dialog->selectedFiles();

    if (file_list.size() > 0) {

      line_edit->setText(file_list[0]);

      emit changed(file_list[0]);
    }
  }
  /// Emits a signal containing the modified path string
  /// from the line edit widget.
  void update_path_from_line_edit() {
    emit changed(line_edit->text());
  }
};

} // namespace

PathSetting* PathSetting::make_java_setting(QWidget* parent) {
  auto* setting = new PathSettingImpl(parent);
  return setting;
}

PathSetting* PathSetting::make_python_setting(QWidget* parent) {
  auto* setting = new PathSettingImpl(parent);
  return setting;
}
