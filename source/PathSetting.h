#pragma once

#include <QWidget>

/// Used to store path settings.
class PathSetting : public QWidget {
  Q_OBJECT
public:
  /// Constructs the path setting for the Java executable.
  /// @param parent A pointer to the parent widget.
  static PathSetting* make_java_setting(QWidget* parent);
  /// Constructs the path setting for the Python executable.
  /// @param parent A pointer to the parent widget.
  static PathSetting* make_python_setting(QWidget* parent);
  /// Constructs the base path setting.
  /// @param parent A pointer to the parent widget.
  PathSetting(QWidget* parent) : QWidget(parent) {}
  /// Just a stub.
  virtual ~PathSetting() {}
  /// Assigns the value contained by the path setting.
  /// @param path The path to assign the setting.
  virtual void set_path(const QString& path) = 0;
signals:
  /// Emitted when the value stored
  /// by the path setting is changed.
  void changed(const QString& path);
};
