#pragma once

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QString;

/// This is the widget that gets displayed if a panic occurs.
class PanicWidget final : public QWidget {
  Q_OBJECT
public:
  /// Constructs the panic widget.
  /// @param parent A pointer to the parent widget.
  PanicWidget(QWidget* parent);
  /// Sets a message describing the panic condition.
  void setText(const QString& msg);
private:
  /// The label describing the condition.
  QLabel* label;
  /// The layout of the panic widget.
  QHBoxLayout* layout;
};
