#include "Action.h"

#include <QJsonArray>
#include <QJsonObject>

bool Action::read(const QJsonValue& value) {

  if (!value.isObject()) {
    return false;
  }

  auto obj = value.toObject();

  animation = obj["animation"].toInt(0);

  return true;
}
