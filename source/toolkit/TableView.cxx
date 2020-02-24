#include "TableView.h"

#include "TableModel.h"

#include <QStyledItemDelegate>
#include <QLineEdit>

namespace herald {

namespace tk {

namespace {

/// This is the class used to ensure name uniqueness.
class ItemNameValidator final : public QValidator {
  /// The index of the item being validated.
  std::size_t item_index;
  /// The model to check uniqueness from.
  TableModel* model;
public:
  /// Constructs a new instance of the validator.
  /// @param i The index of the item being validated.
  /// @param m The model to check for uniqueness from.
  ItemNameValidator(std::size_t i, TableModel* m)
    : item_index(i), model(m) {}
  /// Fixes up a name before commiting it to the table.
  /// This just ensures that the name is unique and non-empty,
  /// modifying it if required.
  void fixup(QString& name) const override {

    if (name.isEmpty()) {
      name = "Texture";
    }

    auto base_name = name;

    for (int i = 0; (i < INT_MAX) && !is_unique(name); i++) {
      name = base_name + " (" + QString::number(i) + ")";
    }
  }
  /// Validates the item name.
  /// @param name The name currently typed.
  QValidator::State validate(QString& name, int&) const override {

    if (name.isEmpty()) {
      return QValidator::Intermediate;
    }

    return is_unique(name) ? QValidator::Acceptable : QValidator::Intermediate;
  }
protected:
  /// Checks if a name is unqiue.
  /// @param name The name to check.
  /// @returns True if the name is unique, false otherwise.
  bool is_unique(const QString& name) const {

    for (std::size_t i = 0; i < model->get_size(); i++) {
      if (i == item_index) {
        continue;
      } else if (model->get_name(i) == name) {
        return false;
      }
    }

    return true;
  }
};

/// A line editor for editing table item names.
/// This editor enforces that the name be unique.
class ItemNameEditor final : public QLineEdit {
public:
  /// Constructs an instance of the name editor.
  /// @param parent A pointer to the parent widget.
  ItemNameEditor(QWidget* parent) : QLineEdit(parent) {
    connect(this, &QLineEdit::textEdited, this, &ItemNameEditor::update_style);
  }
protected:
  /// Updates the style based on whether or not the text is acceptable.
  void update_style() {
    if (hasAcceptableInput()) {
      setStyleSheet("");
    } else {
      setStyleSheet("background-color: red; color: white");
    }
  }
};

/// A custom item delegate to ensure
/// that the name given to an item is unique.
class TableItemDelegate final : public QStyledItemDelegate {
  /// The table model to check name uniqueness with.
  TableModel* model;
public:
  /// Constructs a new delegate instance.
  /// @param m The model of the table to check uniqueness with.
  TableItemDelegate(TableModel* m) : model(m) {
  }
  /// Creates an editor for the item.
  /// @param parent A pointer to the parent widget.
  /// @param index The index of the item getting edited.
  /// @returns A pointer to a new line editor for the item.
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const override {

    auto* validator = new ItemNameValidator((std::size_t) index.row(), model);

    auto* editor = new ItemNameEditor(parent);

    editor->setValidator(validator);

    connect(editor, &ItemNameEditor::editingFinished, this, &TableItemDelegate::commitAndCloseEditor);

    return editor;
  }
  /// Commits the data and closes the editor.
  void commitAndCloseEditor() {
    auto* editor = reinterpret_cast<ItemNameEditor*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
  }
  /// Sets the texture name.
  /// @param editor_widget A pointer to the widget that edited the texture name.
  /// @param index The index of the texture to change the name of.
  void setModelData(QWidget* editor_widget, QAbstractItemModel* model, const QModelIndex& index) const override {

    auto* editor = reinterpret_cast<ItemNameEditor*>(editor_widget);

    model->setData(index, QVariant::fromValue(editor->text()));
  }
};

} // namespace

TableView::TableView(TableModel* m, QWidget* parent) : QListView(parent), model(m) {
  setModel(m);
  setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setItemDelegate(new TableItemDelegate(m));
}

} // namespace tk

} // namespace herald
