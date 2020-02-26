#include "TableEditor.h"

#include "TableButton.h"
#include "TableModel.h"
#include "TableView.h"

#include <herald/ScopedPtr.h>

#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>

namespace herald {

namespace tk {

namespace {

/// Implements the table editor interface.
class TableEditorImpl final : public TableEditor {
  /// The root widget of the table.
  ScopedPtr<QWidget> root_widget;
  /// The table model being modified.
  TableModel* model;
  /// A view of the table being modified.
  ScopedPtr<TableView> view;
  /// The layout of the root widget.
  ScopedPtr<QVBoxLayout> root_layout;
  /// The widget containing the table buttons.
  ScopedPtr<QWidget> button_widget;
  /// The layout of the button widget.
  ScopedPtr<QHBoxLayout> button_layout;
public:
  /// Constructs a new table editor.
  /// @param model_ The model to be modified.
  /// @param parent A pointer to the parent widget.
  TableEditorImpl(TableModel* model_, QWidget* parent) : root_widget(new QWidget(parent)) {

    root_layout = ScopedPtr<QVBoxLayout>(new QVBoxLayout(root_widget.get()));

    model = model_;

    view = ScopedPtr<TableView>(new TableView(model, root_widget.get()));

    button_widget = ScopedPtr<QWidget>(new QWidget(root_widget.get()));
    button_layout = ScopedPtr<QHBoxLayout>(new QHBoxLayout(button_widget.get()));

    root_layout->addWidget(view.get());
    root_layout->addWidget(button_widget.get());

    connect(view.get(), &TableView::selected, this, &TableEditorImpl::handle_item_selected);

    add_button_by_id(TableButtonID::make_predefined(remove_button_id), "Remove");
  }
  /// Adds a button to the table editor.
  /// @param id The ID of the button to add.
  /// @param name The name to assign the button.
  void add_button(std::size_t id, const QString& name) override {
    add_button_by_id(TableButtonID::make(id), name);
  }
  /// Adds a button to the table editor.
  /// @param id The ID of the button to add.
  /// @param name The name to assign the button.
  void add_button_by_id(const TableButtonID& id, const QString& name) {

    auto* button = new TableButton(id, name, button_widget.get());

    connect(button, &TableButton::clicked, this, &TableEditorImpl::handle_button_clicked);

    button_layout->addWidget(button->get_widget());
  }
  /// Accesses a pointer to the editor widget.
  QWidget* get_widget() noexcept override {
    return root_widget.get();
  }
protected:
  /// Handles a clicked item.
  /// @param index The index of the item that was clicked.
  void handle_item_selected(const QModelIndex& index) {
    emit selected((std::size_t) index.row());
  }
  /// Handles a button being clicked.
  /// @param button_id The ID of the button that was clicked.
  void handle_button_clicked(const TableButtonID& button_id) {

    if (button_id.is_predefined()
     && button_id.has_id(TableEditor::remove_button_id)) {
      remove_selected_items();
    }

    emit button_clicked(button_id);
  }
  /// Removes the selected items from the table.
  void remove_selected_items() {
    auto indices = view->selectionModel()->selectedIndexes();
    for (auto index : indices) {
      model->removeRow(index.row());
    }
  }
};

} // namespace

ScopedPtr<TableEditor> TableEditor::make(TableModel* model, QWidget* parent) {
  return new TableEditorImpl(std::move(model), parent);
}

} // namespace tk

} // namespace herald
