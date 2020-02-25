#pragma once

#include <QListView>

namespace herald {

namespace tk {

class TableModel;

/// A derived list view class that's
/// customized specifically for handling
/// the game model tables.
class TableView final : public QListView {
public:
  /// Constructs a new table view.
  /// @param m A pointer to the model that
  /// this table view is made for.
  /// @param parent A pointer to the parent widget.
  TableView(TableModel* m, QWidget* parent);
};

} // namespace tk

} // namespace herald
