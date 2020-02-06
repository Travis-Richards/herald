#include "Writer.h"

#include <QByteArray>

QByteArray Writer::add_player() {
  return QByteArray("add_player\n");
}

QByteArray Writer::build_room() {
  return QByteArray("build_room\n");
}

QByteArray Writer::exit() {
  return QByteArray("exit\n");
}
