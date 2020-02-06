#include "ErrorLog.h"

#include <QTextEdit>
#include <QVBoxLayout>

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

  text_edit->append(line);
}
