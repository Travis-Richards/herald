#include "Writer.h"

#include <QByteArray>

QByteArray Writer::build_room() {
  return QByteArray("build_room\n");
}

QByteArray Writer::exit() {
  return QByteArray("exit\n");
}
