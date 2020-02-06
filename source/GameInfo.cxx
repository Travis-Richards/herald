#include "GameInfo.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

namespace {

/// The implementation of the game info interface.
class GameInfoImpl final : public GameInfo {
  /// The game title.
  QString title;
  /// The author or maker of the game.
  QString author;
  /// The type of the API used to run the game.
  QString api;
  /// A string indicating the error that occurred.
  QString error;
public:
  /// Creates a game information instance as an error object.
  /// @param err A string describing the error.
  /// @param parent A pointer to the parent object.
  static GameInfoImpl* as_error(const QString& err, QObject* parent) {
    auto game_info = new GameInfoImpl(parent);
    game_info->error = err;
    return game_info;
  }
  /// Constructs an instance of the game info implementation.
  /// @param parent A pointer to the parent object.
  GameInfoImpl(QObject* parent) : GameInfo(parent) {}
  /// Reads the game info as a JSON object.
  /// @param object The JSON object to get the info from.
  void read(const QJsonObject& object) {
    title = object["title"].toString();
    author = object["author"].toString();
    api = object["api"].toString();
  }
  /// Indicates if the game info has an error.
  bool has_error() const override {
    return !error.isEmpty();
  }
  /// Accesses the name of the API.
  QString get_api() const override {
    return api;
  }
  /// Accesses the author or maker name.
  QString get_author() const override {
    return author;
  }
  /// Accesses the error of the game info.
  QString get_error() const override {
    return error;
  }
  /// Accesses the title of the game.
  QString get_title() const override {
    return title;
  }
};

} // namespace

GameInfo* GameInfo::open(const QString& game_path, QObject* parent) {

  QString info_path = QDir::cleanPath(game_path + QDir::separator() + "info.json");

  QFile info_file(info_path);

  if (!info_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return GameInfoImpl::as_error(info_file.errorString(), parent);
  }

  QJsonParseError error;

  auto info_doc = QJsonDocument::fromJson(info_file.readAll(), &error);

  if (error.error != QJsonParseError::NoError) {
    return GameInfoImpl::as_error(error.errorString(), parent);
  }

  if (!info_doc.isObject()) {
    return GameInfoImpl::as_error("Root JSON value must be an object", parent);
  }

  auto info_object = info_doc.object();

  auto info = new GameInfoImpl(parent);

  info->read(info_object);

  return info;
}
