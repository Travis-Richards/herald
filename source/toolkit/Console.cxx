#include "Console.h"

#include <herald/ScopedPtr.h>

#include <QFontDatabase>
#include <QTextEdit>

namespace herald {

namespace tk {

namespace {

/// The implementation of the console interface.
class ConsoleImpl final : public Console {
  /// The text edit box containing the console output.
  ScopedPtr<QTextEdit> text_edit;
public:
  /// Constructs a new console instance.
  /// @param parent A pointer to the parent widget.
  ConsoleImpl(QWidget* parent) {
    text_edit = ScopedPtr<QTextEdit>(new QTextEdit(parent));
    text_edit->setReadOnly(true);
    text_edit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
  }
  /// Erases the console content.
  void clear() override {
    text_edit->clear();
  }
  /// Gets a pointer to the console widget.
  /// @returns A pointer to the console widget.
  QWidget* get_widget() noexcept override {
    return text_edit.get();
  }
  /// Logs a process' standard error file data.
  void log_stderr(const QString& str) override {
    print(str, Tag::None);
  }
  /// Logs a process' standard output file data.
  void log_stdout(const QString& str) override {
    print(str, Tag::None);
  }
  /// Prints a message to the console, with a newline.
  void println(const QString& msg, Tag tag) override {
    print(msg + QString('\n'), tag);
  }
protected:
  /// Prints a message to the console.
  /// @param msg The message to print.
  /// This string should contain a newline.
  /// @param tag The tag that describes the message.
  void print(const QString& msg, Tag tag) {
    text_edit->insertPlainText(" ");
    text_edit->insertPlainText(to_string(tag));
    text_edit->insertPlainText(" \uff5c ");
    text_edit->insertPlainText(msg);
  }
  /// Converts a tag into a string.
  /// @param tag The tag to convert to a string.
  /// @returns A human-readable string that
  /// indicates what to expect from the message content.
  QString to_string(Tag tag) {

    const char* exclamation_mark = "\uff01";

    const char* bullet = "\u2022";

    const char* cross_mark = "\u274c";

    const char* check_mark = "\u2713";

    switch (tag) {
      case Tag::None:
        break;
      case Tag::Info:
        return bullet;
      case Tag::Warning:
        return exclamation_mark;
      case Tag::Error:
        return cross_mark;
      case Tag::Success:
        return check_mark;
    }

    return QString(" ");
  }
};

} // namespace

ScopedPtr<Console> Console::make(QWidget* parent) {
  return new ConsoleImpl(parent);
}

} // namespace tk

} // namespace herald
