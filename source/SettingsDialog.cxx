#include "SettingsDialog.h"

#include "PathSetting.h"

#include <QFormLayout>
#include <QSettings>

namespace {

class SettingsDialogImpl;

/// An implementation of the settings dialog interface.
class SettingsDialogImpl final : public SettingsDialog {
  /// The layout of the dialog box.
  QFormLayout* layout;
  /// A pointer to the setting for the Java executable.
  PathSetting* java_setting;
  /// A pointer to the setting for the Python executable.
  PathSetting* python_setting;
public:
  /// Constructs an instance of the settings dialog.
  /// @param parent A pointer to the parent widget.
  SettingsDialogImpl(QWidget* parent) : SettingsDialog(parent) {

    java_setting = PathSetting::make_java_setting(this);
    python_setting = PathSetting::make_python_setting(this);

    connect(java_setting,   &PathSetting::changed, this, &SettingsDialogImpl::update_java_executable);
    connect(python_setting, &PathSetting::changed, this, &SettingsDialogImpl::update_python_executable);

    layout = new QFormLayout(this);
    layout->addRow(tr("Java Interpreter"), java_setting);
    layout->addRow(tr("Python Interpreter"), python_setting);

    setWindowTitle(tr("Settings"));

    resize(640, 480);

    load_settings();
  }
protected slots:
  /// Updates the location of the Java executable.
  /// @param path The path to the interpreter.
  void update_java_executable(const QString& path) {
    QSettings settings;
    settings.setValue("Java", path);
  }
  /// Updates the location of the Python executable.
  /// @param path The path to the interpreter.
  void update_python_executable(const QString& path) {
    QSettings settings;
    settings.setValue("Python", path);
  }
protected:
  /// Loads the settings values.
  void load_settings() {

    QSettings settings;

    java_setting->set_path(settings.value("Java").toString());

    python_setting->set_path(settings.value("Python").toString());
  }
};

} // namespace

SettingsDialog* SettingsDialog::make(QWidget* parent) {
  return new SettingsDialogImpl(parent);
}
