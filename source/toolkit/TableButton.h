#pragma once

#include <QObject>

class QWidget;
class QPushButton;

namespace herald {

namespace tk {

/// A wrapper around a QPushButton widget
/// so that when the button is clicked,
/// it's name is emitted with the signal.
class TableButton final : public QObject {
  Q_OBJECT
public:
  /// Constructs a new instance of the table button.
  /// @param name The name to give the button.
  /// @param parent A pointer to the parent widget.
  TableButton(const QString& name, QWidget* parent);
  /// Accesses the widget containing the button.
  /// @returns A pointer to the button widget.
  QWidget* get_widget() noexcept;
signals:
  /// Emitted when the button is clicked.
  /// @param name The name of the button.
  void clicked(const QString& name);
protected slots:
  /// Handles the button getting clicked.
  void handle_clicked(bool);
private:
  /// The actual button instance.
  QPushButton* button;
};

} // namespace tk

} // namespace herald
