#include "TextureEditor.h"

#include "ProjectManager.h"
#include "ScopedPtr.h"
#include "TableItemEditor.h"

#include <QString>
#include <QStringList>

namespace herald {

namespace tk {

namespace {

class RoomEditor final : public TableItemEditor {
  ProjectManager* manager;
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
    (void)parent;
  }
};

} // namespace

ScopedPtr<TableItemEditor> make_room_editor(ProjectManager* manager) {
  return new RoomEditor(manager);
}

} // namespace tk

} // namespace herald
