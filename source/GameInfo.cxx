#include "GameInfo.h"

#include "Api.h"
#include "ProcessApi.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

namespace {

/// Enumerates the known API types.
enum class ApiType {
  /// No API was specified.
  None,
  /// An arbitrary executable.
  Executable,
  /// The Java API was specified.
  Java,
  /// An unknown API detected.
  Unknown
};

/// Converts the name of an API to
/// an enumerated API type.
ApiType parse_api_name(const QString& name) {
  if (name.isEmpty()) {
    return ApiType::None;
  } else if (name.compare("Executable", Qt::CaseInsensitive) == 0) {
    return ApiType::Executable;
  } else if (name.compare("Java", Qt::CaseInsensitive) == 0) {
    return ApiType::Java;
  } else {
    return ApiType::Unknown;
  }
}

/// Finds a java executable program.
/// @returns A java executable path.
QString find_java() {
  // TODO : Search in:
  //   PATH
  //   JAVA_HOME
#ifdef _WIN32
  return "java.exe";
#else
  return "java";
#endif
}

/// The implementation of the game info interface.
class GameInfoImpl final : public GameInfo {
  /// The root object instance.
  QJsonObject root_object;
  /// Contains any error that might have
  /// occurred while opening the game info.
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
    root_object = object;
  }
  /// Indicates if the game info has an error.
  bool has_error() const override {
    return !error.isEmpty();
  }
  /// Accesses the author or maker name.
  QString get_author() const override {
    return root_object["author"].toString();
  }
  /// Accesses the error of the game info.
  QString get_error() const override {
    return error;
  }
  /// Accesses the title of the game.
  QString get_title() const override {
    return root_object["title"].toString();
  }
  /// Creates an API based off the game info.
  /// @param path The path to run the API from.
  /// @param parent A pointer to the parent object.
  /// @returns An API instance on success, false on failure.
  Api* make_api(const QString& path, QObject* parent) const override;
protected:
  /// Creates a Java API.
  /// @param parent A pointer to the parent object.
  /// @returns A new API instance on success, false on failure.
  Api* make_java_api(const QString& path, QObject* parent) const;
  /// Creates an arbitrary executable API.
  /// @param parent A pointer to the parent object.
  /// @returns A new API instance on success, false on failure.
  Api* make_executable_api(const QString& path, QObject* parent) const;
};

Api* GameInfoImpl::make_api(const QString& path, QObject* parent) const {

  auto api_type = parse_api_name(root_object["api"].toString());
  switch (api_type) {
    case ApiType::None:
      return nullptr;
    case ApiType::Unknown:
      return nullptr;
    case ApiType::Executable:
      return make_executable_api(path, parent);
    case ApiType::Java:
      return make_java_api(path, parent);
  }

  return nullptr;
}

Api* GameInfoImpl::make_java_api(const QString& path, QObject* parent) const {

  auto init_class = root_object["initial_class"].toString();
  if (init_class.isEmpty()) {
    init_class = "Game";
  }

  return make_process_api(find_java(), path, QStringList(init_class), parent);
}

Api* GameInfoImpl::make_executable_api(const QString& path, QObject* parent) const {

  QStringList args;

  auto process_args = root_object["process_args"].toArray();

  for (auto arg : process_args) {
    args << arg.toString();
  }

  return make_process_api(args[0], path, args, parent);
}

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
