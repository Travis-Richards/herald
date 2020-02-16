#pragma once

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class TableItemEditor;

/// Used to edit the items
/// in an arbitrary table.
class TableEditor {
public:
  /// Creates a new table editor.
  /// @param item_editor The class used to display and
  /// edit the contens of a table item.
  /// @param parent A pointer to the parent widget.
  /// @returns A new table editor instance.
  static ScopedPtr<TableEditor> make(ScopedPtr<TableItemEditor>&& item_editor, QWidget* parent);
  /// Just a stub.
  virtual ~TableEditor() {}
  /// Accesses a pointer to the widget.
  /// @returns A pointer to the widget.
  virtual QWidget* get_widget() noexcept = 0;
};

} // namespace tk

} // namespace herald
