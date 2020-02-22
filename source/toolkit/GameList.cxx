#include "GameList.h"

#include <herald/ScopedPtr.h>

#include "GameInfo.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QTableWidget>

namespace herald {

namespace tk {

namespace {

/// The implementation of a game information interface.
class GameInfoImpl final : public GameInfo {
  /// The path to the game.
  QString game_path;
  /// The root JSON object of the info.
  QJsonObject root_object;
public:
  /// Constructs a game information instance from a file.
  /// @param path The path to the game.
  GameInfoImpl(const QString& path) : game_path(path) {

    auto info_path = QDir::cleanPath(game_path + QDir::separator() + "info.json");

    QFile file(info_path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      root_object = QJsonDocument::fromJson(file.readAll()).object();
    }
  }
  /// Fills a row in a table widget with game info.
  /// @param table The table to fill.
  /// @param row The row to put the data into.
  void fill_row(QTableWidget* table, int row) {
    table->setItem(row, 0, new QTableWidgetItem(title()));
    table->setItem(row, 1, new QTableWidgetItem(game_path));
  }
  /// Accesses the API used by the game.
  QString api() const override {
    return root_object["api"].toString();
  }
  /// Accesses the path to the game.
  QString path() const override {
    return game_path;
  }
  /// Accesses the title of the game.
  QString title() const override {
    return root_object["title"].toString();
  }
};

/// The implementation of the game list interface.
class GameListImpl final : public GameList {
  /// A vector of game information instances.
  std::vector<GameInfoImpl> info_vec;
public:
  /// Accesses game information at a specific index.
  const GameInfo* at(std::size_t index) const override {
    if (index >= info_vec.size()) {
      return nullptr;
    } else {
      return &info_vec[index];
    }
  }
  /// Fills a table with the game info.
  void fill_table(QTableWidget* table) override {

    table->setRowCount((int) info_vec.size());

    for (std::size_t i = 0; i < info_vec.size(); i++) {
      info_vec[i].fill_row(table, i);
    }
  }
  /// Opens a game information file.
  bool open(const char* filename) override {
    info_vec.emplace_back(filename);
    return true;
  }
};

/// Removes directories from a directory list that do not exist.
/// @param paths The directory paths to check.
/// @returns A list of directory paths that are valid.
/// A directory path is valid if it exists and it is a directory.
QStringList remove_invalid_dirs(const QStringList& paths) {

  QStringList out;

  for (const auto& path : paths) {
    QFileInfo info(path);
    if (info.exists() && info.isDir()) {
      out << path;
    }
  }

  return out;
}

} // namespace

ScopedPtr<GameInfo> GameInfo::open(const QString& path) {
  return new GameInfoImpl(path);
}

ScopedPtr<GameList> GameList::make() {
  return new GameListImpl();
}

ScopedPtr<GameList> GameList::open(QSettings& settings, bool remove_invalid) {

  auto game_list = GameList::make();

  auto path_list = settings.value("gamelist").toStringList();

  if (remove_invalid) {

    path_list = remove_invalid_dirs(path_list);

    settings.setValue("gamelist", path_list);
  }

  for (const auto& path : path_list) {

    game_list->open(path.toStdString().c_str());
  }

  return game_list;
}

} // namespace tk

} // namespace herald
