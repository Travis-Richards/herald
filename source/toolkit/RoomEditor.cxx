#include "TextureEditor.h"

#include <herald/ScopedPtr.h>

#include "ProjectManager.h"
#include "RoomView.h"
#include "TableItemEditor.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QString>
#include <QStringList>
#include <QWidget>

namespace herald {

namespace tk {

namespace {

class RoomEditor;

/// Used for editing the size of the room.
class SizeEditor final : public QWidget {
public:
  SizeEditor(RoomEditor* room_editor, QWidget* parent) : QWidget(parent) {
    (void)room_editor;
    auto* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Width"), this), 0, 0, Qt::AlignRight);
    layout->addWidget(new QLabel(tr("Height"), this), 1, 0, Qt::AlignRight);
    layout->addWidget(new QSpinBox(this), 0, 1);
    layout->addWidget(new QSpinBox(this), 1, 1);
    layout->addWidget(new QCheckBox(tr("Lock Size"), this), 0, 2, 2, 1);
  }
};

class RoomEditor final : public TableItemEditor {
  /// A view of the room being edited.
  ScopedPtr<RoomView> room_view;
public:
  RoomEditor(ProjectManager*) {}
  QString add() override {
    return "";
  }
  void del(const QString& name) override {
    (void)name;
  }
  QStringList list() const override {
    return QStringList();
  }
  void rename(std::size_t index, const QString& name) override {
    (void)index;
    (void)name;
  }
  void select(const QString& name) override {
    (void)name;
  }
  void setup_widget(QWidget* parent) override {

    room_view = RoomView::make(parent);

    room_view->resize(5, 5);

    parent->layout()->addWidget(room_view->get_widget());
    parent->layout()->addWidget(new SizeEditor(this, parent));
  }
};

} // namespace

ScopedPtr<TableItemEditor> make_room_editor(ProjectManager* manager) {
  return new RoomEditor(manager);
}

} // namespace tk

} // namespace herald
