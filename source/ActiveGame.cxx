#include "ActiveGame.h"

#include <herald/ScopedPtr.h>

#include "Api.h"
#include "ErrorLog.h"
#include "GameInfo.h"
#include "LegacyModelLoader.h"
#include "ModelLoader.h"
#include "ProcessApi.h"
#include "ResponseHandler.h"

#include <herald/Controller.h>
#include <herald/QtEngine.h>
#include <herald/QtTarget.h>

#include <QDir>
#include <QDirIterator>
#include <QGraphicsView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStringList>
#include <QTimer>
#include <QWidget>

namespace {

using namespace herald;

/// Implements the active game interface.
class ActiveGameImpl final : public ActiveGame, public Controller::Observer {
  /// The engine instance that's running the game.
  ScopedPtr<QtEngine> engine;
  /// The window that's displaying the game.
  ScopedPtr<QtTarget> target;
  /// The widget responsible for logging errors.
  ErrorLog* error_log;
  /// The API controlling the game play.
  Api* api;
  /// The timer for frame iteration.
  QTimer timer;
public:
  /// Constructs an instance of the active game.
  /// @param parent A pointer to the parent object.
  ActiveGameImpl(QObject* parent)
    : ActiveGame(parent),
      engine(nullptr),
      error_log(nullptr),
      api(nullptr) {

    timer.setInterval(1000 / 30);

    connect(&timer, &QTimer::timeout, this, &ActiveGameImpl::next_frame);
  }
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
    timer.start();
  }
  /// Pauses the scene animation.
  void pause() override {
    timer.stop();
  }
protected:
  /// Updates axis values.
  void axis_update(double x, double y) override {
    api->update_axis(0, x, y);
  }
  /// Updates a button state.
  void button_update(Button button, bool state) override {
    api->update_button(0, button, state);
  }
  /// Opesn a game at a specific path with supplemental info.
  /// @param path The path to the game to open.
  /// @param info Information regarding the game and how to open it.
  bool open(const QString& path, const GameInfo& info);
  /// Opens the game model.
  /// @param game_path The path to the game directory.
  /// The name of the model file is append to this directory path.
  /// @returns True on success, false on failure.
  bool open_model(const QString& game_path);
  /// Opens a message box and prints a message indicating why
  /// the game failed to open.
  /// @returns Always returns false.
  bool fail(const QString& message);
  /// Goes to the next frame in the game play.
  void next_frame();
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
}

bool ActiveGameImpl::open(const QString& path, const GameInfo& info) {

  target = QtTarget::make(nullptr);

  auto* controller = target->get_controller();
  controller->set_observer(this);

  auto* graphics_view = target->get_graphics_view();
  graphics_view->setWindowTitle(info.get_title());

  engine = QtEngine::make(target.get());

  api = info.make_api(path, engine->get_model(), this);
  if (!api) {
    return fail("Failed to create API instance");
  }

  error_log = new ErrorLog(nullptr);

  connect(api, &Api::error_logged,   error_log, &ErrorLog::log);
  connect(api, &Api::error_occurred, error_log, &ErrorLog::log_fatal);

  open_model(path);

  graphics_view->show();

  return api->start();
}

bool ActiveGameImpl::open_model(const QString& game_path) {

#if 0
  auto actions_path = QDir::cleanPath(game_path + QDir::separator() + "model.json");

  QFile json_file(actions_path);

  if (!json_file.open(QIODevice::ReadOnly)) {
    error_log->warn_open_failure(actions_path, json_file.errorString());
    return false;
  }

  auto json_data = json_file.readAll();

  auto json_doc = QJsonDocument::fromJson(json_data);

  ModelLoader::load(engine->get_model(), json_doc.object(), game_path);

#else
  if (!load_legacy_model(engine->get_model(), game_path)) {
    return false;
  }
#endif

  return true;
}

bool ActiveGameImpl::fail(const QString& message) {
  QMessageBox::critical(nullptr, "Game Creation Error", message);
  return false;
}

void ActiveGameImpl::next_frame() {
  engine->advance((std::size_t) timer.interval());
}

} // namespace

ActiveGame* ActiveGame::make(QObject* parent) {
  return new ActiveGameImpl(parent);
}
