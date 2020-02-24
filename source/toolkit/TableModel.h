#pragma once

#include <QAbstractListModel>

#include <cstring>

namespace herald {

namespace tk {

/// A table model made specifically for tables
/// in the game data model.
class TableModel : public QAbstractListModel {
public:
  /// Gets data at a specific index.
  /// @param index The index to get the data at.
  /// @param role The role that the data will be taking.
  /// @returns The data at the specified index.
  QVariant data(const QModelIndex& index, int role) const override;
  /// Gets the name of an item at a specific index.
  /// This is implemented by the derived class to be table specific.
  /// @param index The index to get the item name at.
  /// @returns The name of the item at the specified index.
  virtual QString get_name(std::size_t index) const = 0;
  /// Gets the size of the table.
  /// @returns The size of the table.
  virtual std::size_t get_size() const = 0;
  /// Gets the row count of the table.
  /// @param parent A index to the parent item.
  /// This is currently ignored.
  int rowCount(const QModelIndex& parent) const override;
  /// Removes rows from the table.
  /// @param row The row to remove items at.
  /// @param count The number of items to remove.
  /// @returns True on success, false on failure.
  bool removeRows(int row, int count, const QModelIndex&) override;
  /// Sets an item of model data.
  /// @param index The index of the item to set the data of.
  /// @param value The value to assign the item.
  /// @param role The role in which @p value is playing.
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
protected:
  /// Accesses flags associated with items in the model.
  /// By default, all items in the model are editable.
  /// @param index The index of the item to get the flags for.
  virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
  /// Removes an item from the table.
  /// @param index The index of the item to remove.
  /// @returns True on success, false on failure.
  virtual bool remove(std::size_t index) = 0;
  /// Sets a data item in the table.
  /// @param index The index of the item to set.
  /// @param value The value to assign the item.
  virtual bool set_data(std::size_t index, const QVariant& value) = 0;
};

} // namespace tk

} // namespace herald
