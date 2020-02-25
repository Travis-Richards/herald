#include "TableModel.h"

namespace herald {

namespace tk {

QVariant TableModel::data(const QModelIndex& index, int role) const {

  switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
      return get_name((std::size_t) index.row());
    default:
      break;
  }

  return QVariant();
}

bool TableModel::removeRows(int row, int count, const QModelIndex& parent) {

  beginRemoveRows(parent, row, row + count);

  auto success = true;

  for (int i = 0; i < count; i++) {
    success &= remove((std::size_t) (row + i));
  }

  endRemoveRows();

  return success;
}

int TableModel::rowCount(const QModelIndex&) const {
  return (int) get_size();
}

bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role) {

  if (role == Qt::EditRole) {
    rename((std::size_t) index.row(), value.toString());
  }

  return true;
}

Qt::ItemFlags TableModel::flags(const QModelIndex&) const {
  return Qt::ItemIsSelectable
       | Qt::ItemIsEditable
       | Qt::ItemIsEnabled;
}

} // namespace tk

} // namespace herald
