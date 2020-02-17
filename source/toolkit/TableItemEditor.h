#pragma once

#include <cstddef>

class QWidget;
class QString;
class QStringList;

namespace herald {

namespace tk {

/// This is the interface for a table
/// item editor. A table item editor is
/// used for editing the contents of a table.
/// For example, in the texture table, there
/// is a table item editor which displays the
/// chosen texture and allows for simple
/// image editing.
class TableItemEditor {
public:
  /// Just a stub.
  virtual ~TableItemEditor() {}
  /// Called when an item is added to the table.
  /// @returns A name to give the new item.
  /// This may just be an empty string.
  virtual QString add() = 0;
  /// Deletes an item from the table.
  /// @param name The name of the item to delete.
  virtual void del(const QString& name) = 0;
  /// Lists the items labels in the table data.
  /// @returns The items labels table data.
  virtual QStringList list() = 0;
  /// Renames an item from the list.
  /// @param index The index of the item that was renamed.
  /// @param name The name given to the item.
  virtual void rename(std::size_t index, const QString& name) = 0;
  /// Selects an item from the table.
  /// This usually should lead to the item being displayed.
  /// @param name The name of the selected item.
  virtual void select(const QString& name) = 0;
  /// Sets up the widget used to display
  /// the table item contents.
  /// @param widget A pointer to the widget to setup.
  virtual void setup_widget(QWidget* widget) = 0;
};

} // namespace tk

} // namespace herald
