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
  /// Logs a fatal message to the error log.
  /// @param message The message to put into the log.
  void log_fatal(const QString& message);
  /// Warns about a file that failed to open.
  /// @param filename The name of the file to open.
  /// @param error A string describing the reason the file couldn't open.
  /// This may be an empty string.
  void warn_open_failure(const QString& filename, const QString& error);
private:
  /// The text edit widget to put the messages into.
  QTextEdit* text_edit;
};
