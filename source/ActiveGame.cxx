#include "ActiveGame.h"

#include "Api.h"
#include "ErrorLog.h"
#include "GameInfo.h"
#include "ProcessApi.h"
#include "Scene.h"
#include "SceneView.h"

#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>

namespace {

/// Enumerates the known API types.
enum class ApiType {
  /// No API was specified.
  None,
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

/// Implements the active game interface.
class ActiveGameImpl final : public ActiveGame {
  /// The widget responsible for logging errors.
  ErrorLog* error_log;
  /// A view of the game scene.
  SceneView* scene_view;
  /// The game scene being rendered.
  Scene* scene;
  /// The API controlling the game play.
  Api* api;
public:
  /// Constructs an instance of the active game.
  /// @param parent A pointer to the parent object.
  ActiveGameImpl(QObject* parent)
    : ActiveGame(parent),
      error_log(nullptr),
      scene_view(nullptr),
      scene(nullptr),
      api(nullptr) {}
  /// Releases memory allocated by the game.
  ~ActiveGameImpl();
  /// Closes the game, if it's running.
  void close() override;
  /// Opens the game at a specified path.
  /// @param path The path to open the game at.
  /// @returns True on success, false on failure.
  bool open(const QString& path) override;
protected:
  /// Opesn a game at a specific path with supplemental info.
  /// @param path The path to the game to open.
  /// @param info Information regarding the game and how to open it.
  bool open(const QString& path, const GameInfo& info);
  /// Opens the textures to be used in the game.
  /// @param path The path to the game.
  /// @returns True on success, false on failure.
  bool open_textures(const QString& path);
  /// Opens a message box and prints a message indicating why
  /// the game failed to open.
  /// @returns Always returns false.
  bool fail(const QString& message);
  /// Handles closing of the scene view.
  void handle_scene_view_closing();
};

ActiveGameImpl::~ActiveGameImpl() {
  close();
}

bool ActiveGameImpl::open(const QString& path) {

  auto game_info = GameInfo::open(path, this);
  if (!game_info) {
    return fail("Missing 'info.json' file");
  } else if (game_info->has_error()) {
    return fail("Failed to open game info (" + game_info->get_error() + ")");
  }

  auto success = open(path, *game_info);

  delete game_info;

  return success;
}

void ActiveGameImpl::close() {

  if (api) {
    api->exit();
    delete api;
    api = nullptr;
  }

  if (error_log) {
    delete error_log;
    error_log = nullptr;
  }

  if (scene) {
    delete scene;
    scene = nullptr;
  }
}

bool ActiveGameImpl::open(const QString& path, const GameInfo& info) {

  auto api_type = parse_api_name(info.get_api());
  switch (api_type) {
    case ApiType::None:
      return fail("No API was specified in 'info.txt'");
    case ApiType::Unknown:
      return fail("Unknown API '" + info.get_api() + "'");
    case ApiType::Java:
      api = make_process_api(find_java(), path, QStringList("Game"), this);
      break;
  }

  if (!api) {
    return fail("Failed to create " + info.get_api() + " API");
  } else if (!api->init_game()) {
    return fail("Failed to initialize game");
  }

  scene = Scene::make(this);

  scene_view = new SceneView(scene);
  scene_view->setWindowTitle(info.get_title());

  connect(scene_view, &SceneView::closing, this, &ActiveGameImpl::handle_scene_view_closing);

  connect(scene_view, &SceneView::resized, scene, &Scene::resize);

  error_log = new ErrorLog(nullptr);

  connect(api, &Api::error_log, error_log, &ErrorLog::log);

  scene_view->show();

  open_textures(path);

  return api->build_room(scene);
}

bool ActiveGameImpl::open_textures(const QString& path) {

  auto textures_path = QDir::cleanPath(path + QDir::separator() + "textures"); 

  QDirIterator it(textures_path, QDir::Files);

  QStringList texture_list;

  while (it.hasNext()) {
    texture_list << it.next();
  }

  texture_list.sort();

  for (auto texture_path : texture_list) {
    scene->load_texture(texture_path);
  }

  return true;
}

bool ActiveGameImpl::fail(const QString& message) {
  QMessageBox::critical(nullptr, "Game Creation Error", message);
  return false;
}

void ActiveGameImpl::handle_scene_view_closing() {
  emit closing(this);
}

} // namespace

ActiveGame* ActiveGame::make(QObject* parent) {
  return new ActiveGameImpl(parent);
}
