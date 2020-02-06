#pragma once

#include <QWidget>

class QTextEdit;

/// The error log is for errors emitted
/// from the game. It doesn't show up unless
/// the game prints an error.
class ErrorLog : public QWidget {
public:
  /// Constructs an error log instance.
  /// @param widget A pointer to the parent widget.
  ErrorLog(QWidget* parent);
public slots:
  /// Logs a message into the error log.
  /// @param message The message to put onto the log.
  void log(const QString& message);
private:
  /// The text edit widget to put the messages into.
  QTextEdit* text_edit;
};
