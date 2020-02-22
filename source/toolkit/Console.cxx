#include "Console.h"

#include <herald/ScopedPtr.h>

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
  }
  /// Gets a pointer to the console widget.
  /// @returns A pointer to the console widget.
  QWidget* get_widget() noexcept override {
    return text_edit.get();
  }
  /// Prints a message to the console, with a newline.
  void println(const QString& msg) override {
    text_edit->append(msg);
    text_edit->append(QString('\n'));
  }
};

} // namespace

ScopedPtr<Console> Console::make(QWidget* parent) {
  return new ConsoleImpl(parent);
}

} // namespace tk

} // namespace herald
