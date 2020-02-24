#include "TableButton.h"

#include <QPushButton>

namespace herald {

namespace tk {

TableButton::TableButton(const QString& name, QWidget* parent) : QObject(parent) {

  button = new QPushButton(name, parent);

  connect(button, &QPushButton::clicked, this, &TableButton::handle_clicked);
}

QWidget* TableButton::get_widget() noexcept {
  return button;
}

void TableButton::handle_clicked(bool) {
  emit clicked(button->text());
}

} // namespace tk

} // namespace herald
