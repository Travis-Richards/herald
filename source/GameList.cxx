#include "GameList.h"

#include <QSettings>
#include <QString>
#include <QStringList>

GameList::GameList(QObject* parent) : QObject(parent), path_list(new QStringList) {

}

GameList::~GameList() {
  delete path_list;
}

void GameList::add(const QString& path) {
  path_list->append(path);
  path_list->removeDuplicates();
  path_list->sort();
}

void GameList::remove(int index) {
  path_list->removeAt(index);
}

void GameList::load(QSettings& settings) {
  *path_list = settings.value("gamelist").value<QStringList>();
}

QString GameList::at(int index) const {
  return path_list->at(index);
}

int GameList::size() const {
  return path_list->size();
}

void GameList::store(QSettings& settings) const {
  settings.setValue("gamelist", *path_list);
}
