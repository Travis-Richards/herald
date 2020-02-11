#pragma once

#include <QDialog>

/// Used to allow the user to edit engine settings.
class SettingsDialog : public QDialog {
  Q_OBJECT
public:
  /// Creates a new settings dialog instance.
  /// @param parent A pointer to the parent widget.
  static SettingsDialog* make(QWidget* parent);
  /// Constructs the base of the settings dialog.
  /// @param parent A pointer to the parent widget, if any.
  SettingsDialog(QWidget* parent) : QDialog(parent) {}
  /// Just a stub.
  virtual ~SettingsDialog() {}
};
