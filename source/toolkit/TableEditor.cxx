#include "TableEditor.h"

#include <herald/ScopedPtr.h>

#include "TableItemEditor.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>

namespace herald {

namespace tk {

namespace {

/// Implements the table editor interface.
class TableEditorImpl final : public TableEditor {
  /// The root widget for the table editor.
  ScopedPtr<QWidget> root_widget;
  /// The table item editor.
  ScopedPtr<TableItemEditor> item_editor;
  /// The list containing the table items.
  ScopedPtr<QListWidget> list_widget;
public:
  /// Constructs a new table editor.
  /// @param parent A pointer to the parent widget.
  TableEditorImpl(ScopedPtr<TableItemEditor>&& item_editor_, QWidget* parent)
    : root_widget(new QWidget(parent)), item_editor(std::move(item_editor_)) {

    auto* layout = new QHBoxLayout(root_widget.get());
    make_l_widget(layout, root_widget.get());
    make_r_widget(layout, root_widget.get());
    fill_table();
  }
  /// Accesses a pointer to the editor widget.
  QWidget* get_widget() noexcept override {
    return root_widget.get();
  }
protected:
  /// Creates the left hand widget.
  /// @param layout The layout to add the widget to.
  /// @param parent The parent widget.
  void make_l_widget(QLayout* layout, QWidget* parent) {

    auto* left_widget = new QWidget(parent);

    auto* left_layout = new QVBoxLayout(left_widget);

    list_widget = ScopedPtr<QListWidget>(new QListWidget(left_widget));

    list_widget->setEditTriggers(QAbstractItemView::DoubleClicked);

    auto select_functor = [this](const QString& text) {
      item_editor->select(text);
    };

    auto rename_functor = [this](QListWidgetItem* item) {
      item_editor->rename(list_widget->row(item), item->text());
    };

    QObject::connect(list_widget.get(), &QListWidget::currentTextChanged, select_functor);
    QObject::connect(list_widget.get(), &QListWidget::itemChanged, rename_functor);

    left_layout->addWidget(list_widget.get());
    left_layout->addWidget(make_buttons_widget(left_widget));

    layout->addWidget(left_widget);
  }
  /// Creates the table modification buttons.
  /// @param parent A pointer to the parent widget.
  /// @returns A pointer to the new buttons widget.
  QWidget* make_buttons_widget(QWidget* parent) {

    auto* buttons_widget = new QWidget(parent);

    auto* buttons_layout = new QHBoxLayout(buttons_widget);

    auto* add_button = new QPushButton(QObject::tr("Add"),    buttons_widget);
    auto* del_button = new QPushButton(QObject::tr("Delete"), buttons_widget);

    auto del_functor = [this](bool) {
      auto* current_item = list_widget->currentItem();
      if (current_item) {
        item_editor->del(current_item->text());
        list_widget->takeItem(list_widget->row(current_item));
      }
    };

    auto add_functor = [this](bool) {

      auto item_name = item_editor->add();

      auto* item_widget = new QListWidgetItem(list_widget.get());
      item_widget->setText(item_name);
      item_widget->setFlags(item_widget->flags() | Qt::ItemIsEditable);

      list_widget->addItem(item_widget);
    };

    QObject::connect(del_button, &QPushButton::clicked, del_functor);
    QObject::connect(add_button, &QPushButton::clicked, add_functor);

    buttons_layout->addWidget(add_button);
    buttons_layout->addWidget(del_button);

    return buttons_widget;
  }
  /// Creates the right hand widget.
  /// @param layout The layout to add the widget to.
  /// @param parent A pointer to the parent widget.
  void make_r_widget(QLayout* layout, QWidget* parent) {

    auto* r_widget = new QWidget(parent);

    r_widget->setLayout(new QVBoxLayout);

    layout->addWidget(r_widget);

    item_editor->setup_widget(r_widget);
  }
  /// Fills the table with previously existing items.
  void fill_table() {

    auto items = item_editor->list();

    list_widget->clear();

    for (const auto& item : items) {

      auto* item_widget = new QListWidgetItem(list_widget.get());
      item_widget->setText(item);
      item_widget->setFlags(item_widget->flags() | Qt::ItemIsEditable);

      list_widget->addItem(item_widget);
    }
  }
};

} // namespace

ScopedPtr<TableEditor> TableEditor::make(ScopedPtr<TableItemEditor>&& item_editor, QWidget* parent) {
  return new TableEditorImpl(std::move(item_editor), parent);
}

} // namespace tk

} // namespace herald
