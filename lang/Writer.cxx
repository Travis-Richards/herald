#include "Writer.h"

#include <QByteArray>

QByteArray Writer::build_menu() {
  return QByteArray("build_menu\n");
}

QByteArray Writer::exit() {
  return QByteArray("exit\n");
}
