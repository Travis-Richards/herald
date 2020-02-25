#include "TableButton.h"

#include <QPushButton>

namespace herald {

namespace tk {

TableButton::TableButton(const TableButtonID& id_, const QString& name, QWidget* parent) : QObject(parent), id(id_) {

  button = new QPushButton(name, parent);

  connect(button, &QPushButton::clicked, this, &TableButton::handle_clicked);
}

QWidget* TableButton::get_widget() noexcept {
  return button;
}

void TableButton::handle_clicked(bool) {
  emit clicked(id);
}

} // namespace tk

} // namespace herald
