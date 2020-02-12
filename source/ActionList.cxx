#include "ActionList.h"

#include "Action.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QVector>

namespace {

/// Implements the action list interface.
class ActionListImpl final : public ActionList {
  /// The actions part of the list.
  QVector<Action> actions;
public:
  /// Constructs a new action list instance.
  /// @param parent A pointer to the parent object.
  ActionListImpl(QObject* parent) : ActionList(parent) {}
  /// Accesses an action from the list.
  const Action* at(int index) const noexcept override {
    if (index < 0) {
      return nullptr;
    } else if (index >= actions.size()) {
      return nullptr;
    } else {
      return &actions[index];
    }
  }
  /// Reads the list from a JSON array.
  /// @returns True on success, false on failure.
  bool read(const QJsonValue& json_actions) override {

    if (!json_actions.isArray()) {
      return false;
    }

    auto arr = json_actions.toArray();

    for (auto element : arr) {

      Action action;

      if (!action.read(element)) {
        return false;
      } else {
        actions.push_back(action);
      }
    }

    return true;
  }
  /// Indicates the number of actions in the list.
  int size() const noexcept override {
    return actions.size();
  }
};

} // namespace

ActionList* ActionList::make(QObject* parent) {
  return new ActionListImpl(parent);
}
