#include "Writer.h"

#include <QByteArray>
#include <QTextStream>

QByteArray Writer::add_player() {
  return QByteArray("add_player\n");
}

QByteArray Writer::build_room() {
  return QByteArray("build_room\n");
}

QByteArray Writer::exit() {
  return QByteArray("exit\n");
}

QByteArray Writer::build_object_map() {
  return QByteArray("build_object_map\n");
}

QByteArray Writer::set_background() {
  return QByteArray("set_background\n");
}

QByteArray Writer::update_axis(int c, double x, double y) {

  QByteArray output;

  QTextStream stream(&output);
  stream << "update_axis" << endl;
  stream << c << endl;
  stream << x << endl;
  stream << y << endl;

  return output;
}

QByteArray Writer::update_button(int c, int b, bool state) {

  QByteArray output;

  QTextStream stream(&output);
  stream << "update_button" << endl;
  stream << c << endl;
  stream << b << endl;
  stream << (state ? "true" : "false") << endl;

  return output;
}
