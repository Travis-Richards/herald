#pragma once

#include <QObject>

class Action;
class QJsonValue;

/// Contains a list of actions,
/// usually from an external file.
class ActionList : public QObject {
public:
  /// Creates a new action list.
  /// @param parent A pointer to a parent object.
  /// @returns A new action list instance.
  static ActionList* make(QObject* parent);
  /// Constructs a new action list instance.
  /// @param parent A pointer to the parent object.
  ActionList(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~ActionList() {}
  /// Accesses an action at a specific index.
  /// @param index The index of the action to get.
  /// @returns A pointer to a action at the specified index.
  /// If the index is out of bounds, then a null pointer
  /// is returned instead.
  virtual const Action* at(int index) const noexcept = 0;
  /// Reads an action list from a JSON value.
  /// @param json_actions The JSON actions definition.
  /// @returns True on success, false on failure.
  virtual bool read(const QJsonValue& json_actions) = 0;
  /// Indicates the number of actions in the list.
  virtual int size() const noexcept = 0;
};
