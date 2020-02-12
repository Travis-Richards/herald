#include "ErrorLog.h"

#include <QTextEdit>
#include <QVBoxLayout>

namespace {

/// Enumerates the severity
/// of an error message.
enum class Severity {
  Warning,
  FatalError
};

/// Formats a message as HTML.
/// @param severity The severity of the message.
/// @param message A message describing the error.
QString format(Severity severity, const QString& message) {

  QString header = "<b>";

  switch (severity) {
    case Severity::Warning:
      header += "<font color=\"magenta\">";
      header += "warning: ";
      break;
    case Severity::FatalError:
      header += "<font color=\"red\">";
      header += "fatal error: ";
      break;
  }

  header += "</font>";

  return header + message + "</b>";
}

} // namespace

ErrorLog::ErrorLog(QWidget* parent) : QWidget(parent) {

  setWindowTitle("Error Log");

  text_edit = new QTextEdit(this);
  text_edit->setReadOnly(true);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(text_edit);

  setLayout(layout);
}

void ErrorLog::log(const QString& line) {

  if (!line.isEmpty() && isHidden()) {
    show();
  }

  text_edit->textCursor().insertText(line);
}

void ErrorLog::log_fatal(const QString& line) {

  if (!line.isEmpty() && isHidden()) {
    show();
  }

  text_edit->textCursor().insertHtml(format(Severity::FatalError, line));
}

void ErrorLog::warn_open_failure(const QString& filename, const QString& error) {
  QString message;
  message += "Failed to open '";
  message += filename;
  message += "' (";
  message += error;
  message += ')';
  text_edit->textCursor().insertHtml(format(Severity::Warning, message));
}
