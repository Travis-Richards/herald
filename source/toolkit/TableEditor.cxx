#include "TableEditor.h"

#include "ScopedPtr.h"
#include "TableItemEditor.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>

#include <QDebug>

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

    auto add_functor = [this](bool) {

      auto item_name = item_editor->add();

      list_widget->addItem(item_name);
    };

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

    qDebug() << "here: " << items;

    list_widget->clear();

    for (const auto& item : items) {
      list_widget->addItem(item);
    }
  }
};

} // namespace

ScopedPtr<TableEditor> TableEditor::make(ScopedPtr<TableItemEditor>&& item_editor, QWidget* parent) {
  return new TableEditorImpl(std::move(item_editor), parent);
}

} // namespace tk

} // namespace herald
