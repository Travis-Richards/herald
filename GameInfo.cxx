#include "GameInfo.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>

GameInfo* GameInfo::open(const QString& game_path, QObject* parent) {

  QString info_path = QDir::cleanPath(game_path + QDir::separator() + "info.txt");

  QFile info_file(info_path);

  if (!info_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return nullptr;
  }

  auto info = new GameInfo(parent);

  QTextStream info_stream(&info_file);

  *info->title = info_stream.readLine();
  *info->author = info_stream.readLine();
  *info->api = info_stream.readLine();

  return info;
}

GameInfo::GameInfo(QObject* parent) : QObject(parent) {
  api = new QString;
  author = new QString;
  title = new QString;
}

QString GameInfo::get_api() const {
  return *api;
}

QString GameInfo::get_author() const {
  return *author;
}

QString GameInfo::get_title() const {
  return *title;
}
