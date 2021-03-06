#include "GameInfo.h"

#include <herald/ScopedPtr.h>

#include "Api.h"
#include "ProcessApi.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QString>
#include <QStringList>

using namespace herald;

namespace {

/// Enumerates the known API types.
enum class ApiType {
  /// No API was specified.
  None,
  /// An arbitrary executable.
  Executable,
  /// The Java API was specified.
  Java,
  /// The Python API was specified.
  Python,
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
  } else if (name.compare("Python", Qt::CaseInsensitive) == 0) {
    return ApiType::Python;
  } else {
    return ApiType::Unknown;
  }
}

/// Turns a base program name
/// into an executable filename.
/// On non-Windows platforms, this does nothing.
/// @param base The base of the filename.
/// @returns The resultant executable filename for @p base.
QString executable_name(const QString& base) {
#ifdef _WIN32
  return base + ".exe";
#else
  return base;
#endif
}

/// Finds a java executable program.
/// @returns A java executable path.
QString find_java() {
  QSettings settings;
  auto java_setting = settings.value("Java").toString();
  if (java_setting.isEmpty()) {
    return executable_name("java");
  } else {
    return java_setting;
  }
}

/// Finds a java executable program.
/// @returns A java executable path.
QString find_python() {
  QSettings settings;
  auto python_setting = settings.value("Python").toString();
  if (python_setting.isEmpty()) {
    return executable_name("python");
  } else {
    return python_setting;
  }
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
  /// @param m The model to be modified the the game API.
  /// @param parent A pointer to the parent object.
  /// @returns An API instance on success, false on failure.
  Api* make_api(const QString& path, Model* m, QObject* parent) const override;
protected:
  /// Creates a Java API.
  /// @param m The model to be modified the the game API.
  /// @param parent A pointer to the parent object.
  /// @returns A new API instance on success, a null pointer on failure.
  Api* make_java_api(const QString& path, Model* m, QObject* parent) const;
  /// Creates a Python API.
  /// @param path The path of the game directory.
  /// @param m The model to be modified the the game API.
  /// @param parent A pointer to the parent object.
  /// @returns A new API instance on success, a null pointer on failure.
  Api* make_python_api(const QString& path, Model* m, QObject* parent) const;
  /// Creates an arbitrary executable API.
  /// @param path The path of the game directory.
  /// @param m The model to be modified the the game API.
  /// @param parent A pointer to the parent object.
  /// @returns A new API instance on success, a null pointer on failure.
  Api* make_executable_api(const QString& path, Model* m, QObject* parent) const;
};

Api* GameInfoImpl::make_api(const QString& path, Model* m, QObject* parent) const {

  auto api_type = parse_api_name(root_object["api"].toString());
  switch (api_type) {
    case ApiType::None:
      return nullptr;
    case ApiType::Unknown:
      return nullptr;
    case ApiType::Executable:
      return make_executable_api(path, m, parent);
    case ApiType::Java:
      return make_java_api(path, m, parent);
    case ApiType::Python:
      return make_python_api(path, m, parent);
  }

  return nullptr;
}

Api* GameInfoImpl::make_java_api(const QString& path, Model* m, QObject* parent) const {

  auto init_class = root_object["initial_class"].toString();
  if (init_class.isEmpty()) {
    init_class = "Game";
  }

  auto api_factory = herald::ProcessApiFactory::make();
  api_factory->set_model(m);
  api_factory->set_program(find_java());
  api_factory->set_working_directory(path);
  api_factory->set_args(QStringList(init_class));
  return api_factory->make_process_api(parent);
}

Api* GameInfoImpl::make_python_api(const QString& path, Model* m, QObject* parent) const {
  auto api_factory = herald::ProcessApiFactory::make();
  api_factory->set_model(m);
  api_factory->set_program(find_python());
  api_factory->set_working_directory(path);
  return api_factory->make_process_api(parent);
}

Api* GameInfoImpl::make_executable_api(const QString& path, Model* m, QObject* parent) const {

  QStringList args;

  auto program = QDir::cleanPath(QString(".") + QDir::separator() + executable_name("game"));

  args << program;

  auto process_args = root_object["process_args"].toArray();

  for (auto arg : process_args) {
    args << arg.toString();
  }

  auto api_factory = herald::ProcessApiFactory::make();
  api_factory->set_args(args);
  api_factory->set_model(m);
  api_factory->set_program(program);
  api_factory->set_working_directory(path);
  return api_factory->make_process_api(parent);
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
