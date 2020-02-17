#include "TextureEditor.h"

#include "ProjectManager.h"
#include "RoomView.h"
#include "ScopedPtr.h"
#include "TableItemEditor.h"

#include <QLayout>
#include <QString>
#include <QStringList>
#include <QWidget>

namespace herald {

namespace tk {

namespace {

class RoomEditor final : public TableItemEditor {
  ProjectManager* manager;
  /// A view of the room being edited.
  ScopedPtr<RoomView> room_view;
public:
  RoomEditor(ProjectManager* m) : manager(m) {}
  QString add() override {
    return "";
  }
  void del(const QString& name) override {
    (void)name;
  }
  QStringList list() override {
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

    parent->layout()->addWidget(room_view->get_widget());
  }
};

} // namespace

ScopedPtr<TableItemEditor> make_room_editor(ProjectManager* manager) {
  return new RoomEditor(manager);
}

} // namespace tk

} // namespace herald
