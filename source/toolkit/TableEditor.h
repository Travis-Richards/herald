#pragma once

#include <QObject>

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class TableButtonID;
class TableModel;

/// Used to edit the items
/// in an arbitrary table.
class TableEditor : public QObject {
  Q_OBJECT
public:
  /// This is the ID for the delete button.
  static constexpr std::size_t remove_button_id = 0;
  /// Creates a new table editor.
  /// @param model The table model to be edited.
  /// @param parent A pointer to the parent widget.
  /// @returns A new table editor instance.
  static ScopedPtr<TableEditor> make(TableModel* model, QWidget* parent);
  /// Just a stub.
  virtual ~TableEditor() {}
  /// Adds a button to the table editor.
  /// @param id The ID to assign the button.
  /// @param name The name to give the button.
  virtual void add_button(std::size_t id, const QString& name) = 0;
  /// Accesses a pointer to the root widget of the table editor.
  /// @returns A pointer to the root widget.
  virtual QWidget* get_widget() noexcept = 0;
signals:
  /// Emitted when a table button is clicked.
  /// @param button_id The ID of the button that was clicked.
  void button_clicked(const TableButtonID& button_id);
  /// This signal is emitted when
  /// an item from the table is selected
  /// for display (or editing.)
  /// @param index The index of the item
  /// that was selected.
  void selected(std::size_t index);
protected:
  /// Constructs the base table editor.
  /// @param parent A pointer to the parent object.
  TableEditor() : QObject() {}
};

} // namespace tk

} // namespace herald
