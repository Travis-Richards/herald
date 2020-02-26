#pragma once

#include <QListView>

namespace herald {

namespace tk {

class TableModel;

/// A derived list view class that's
/// customized specifically for handling
/// the game model tables.
class TableView final : public QListView {
  Q_OBJECT
public:
  /// Constructs a new table view.
  /// @param m A pointer to the model that
  /// this table view is made for.
  /// @param parent A pointer to the parent widget.
  TableView(TableModel* m, QWidget* parent);
signals:
  /// This signal is emitted when an item
  /// from the table is selected.
  /// @param index The index of the item that was selected.
  void selected(const QModelIndex& index);
protected:
  /// Overide to get the new item index.
  void currentChanged(const QModelIndex& selected,
                      const QModelIndex& deselected) override;
};

} // namespace tk

} // namespace herald
