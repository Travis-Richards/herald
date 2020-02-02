#include "Engine.h"

#include "Api.h"
#include "Config.h"
#include "GameInfo.h"
#include "GameList.h"
#include "ProcessApi.h"

#include <QSettings>
#include <QString>
#include <QStringList>

Engine::Engine(Scene* scene_, QObject* parent)
    : QObject(parent),
      api(nullptr),
      game_list(new GameList(this)),
      scene(scene_) {

  last_selected = -1;

  if (api) {
    QObject::connect(api, &Api::symbol_not_found, this, &Engine::handle_missing_symbol);
  }
}

Engine::~Engine() {

}

void Engine::select_game(int index) {
  last_selected = index;
}

void Engine::open_game(const QString& path) {
  game_list->add(path);
  emit updated_game_list(*game_list);
}

void Engine::close_game() {
  if (api) {
    api->exit();
    delete api;
    api = nullptr;
  }
}

void Engine::play_game() {

  if (last_selected < 0) {
    return;
  }

  auto game_path = game_list->at(last_selected);
  if (game_path.isEmpty()) {
    return;
  }

  auto game_info = GameInfo::open(game_path, this);
  if (!game_info) {
    return;
  }

  if (game_info->get_api() == "Java") {
    api = make_java_api(game_path);
  } else {
    return;
  }

  api->init_game();

  api->build_menu(scene);

  emit game_started(game_info->get_title());

  delete game_info;
}

void Engine::handle_exit() {

  store_settings();

  if (api) {
    api->exit();
  }
}

bool Engine::init_game() {

  if (!api) {
    return false;
  }

  return api->init_game();
}

bool Engine::load_settings() {
  QSettings settings;
  game_list->load(settings);
  emit updated_game_list(*game_list);
  return true;
}

bool Engine::store_settings() {
  QSettings settings;
  game_list->store(settings);
  return true;
}

bool Engine::load_level(int id, Scene* scene) {

  if (!api) {
    return false;
  }

  return api->build_level(id, scene);
}

int Engine::get_level_count() {

  if (!api) {
    return 0;
  }

  return api->get_level_count();
}

void Engine::handle_missing_symbol(SymbolType type, const char* name) {
  QString msg;
  msg += to_string(type);
  msg += " named <b>";
  msg += name;
  msg += "</b> not found.";
  emit panic(msg);
}

Api* Engine::make_java_api(const QString& path) {

  QStringList args;
  args << "Game";

#ifdef _WIN32
  return make_process_api("java.exe", path, args, this);
#else
  return make_process_api("java", path, args, this);
#endif
}
