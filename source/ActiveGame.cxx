#include "ActiveGame.h"

#include "Api.h"
#include "Controller.h"
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
  /// Starts the scene animation.
  void start() override {
    if (scene) {
      scene->start();
    }
  }
  /// Pauses the scene animation.
  void pause() override {
    if (scene) {
      scene->pause();
    }
  }
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
protected slots:
  /// Handles closing of the scene view.
  void handle_scene_view_closing();
  /// Handles completion of the background modification.
  void handle_set_background();
  /// Handles the completion of a room construction.
  void handle_built_room();
  /// Handles the case where the objects
  /// are done being added to the scene.
  void handle_built_object_map();
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

  api = info.make_api(path, this);
  if (!api) {
    return fail("Failed to create API instance");
  }

  scene = Scene::make(this);

  scene_view = SceneView::make(scene);
  scene_view->setWindowTitle(info.get_title());

  connect(scene_view, &SceneView::closing, this, &ActiveGameImpl::handle_scene_view_closing);
  connect(scene_view, &SceneView::resized, scene, &Scene::resize);

  auto* controller = scene_view->get_controller();
  connect(controller, &Controller::update_axis,   api, &Api::update_def_axis);
  connect(controller, &Controller::update_button, api, &Api::update_def_button);

  error_log = new ErrorLog(nullptr);

  connect(api, &Api::error_logged, error_log, &ErrorLog::log);
  connect(api, &Api::error_occurred, error_log, &ErrorLog::log_fatal);
  connect(api, &Api::background_set, this, &ActiveGameImpl::handle_set_background);
  connect(api, &Api::room_built, this, &ActiveGameImpl::handle_built_room);
  connect(api, &Api::object_map_built, this, &ActiveGameImpl::handle_built_object_map);

  scene_view->show();

  open_textures(path);

  return api->set_background(scene);
}

bool ActiveGameImpl::open_textures(const QString& path) {

  auto textures_path = QDir::cleanPath(path + QDir::separator() + "textures"); 

  QDirIterator it(textures_path, QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Dirs);

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

void ActiveGameImpl::handle_set_background() {
  api->build_room(scene);
}

void ActiveGameImpl::handle_built_room() {
  api->fill_objects(scene);
}

void ActiveGameImpl::handle_built_object_map() {
  scene->update_view();
}

} // namespace

ActiveGame* ActiveGame::make(QObject* parent) {
  return new ActiveGameImpl(parent);
}
