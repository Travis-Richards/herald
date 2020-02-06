#include "Engine.h"

#include "ActiveGame.h"
#include "ActiveGameList.h"
#include "GameList.h"

#include <QSettings>
#include <QString>
#include <QStringList>

Engine::Engine(QObject* parent) : QObject(parent), active_games(nullptr), game_list(nullptr) {

  active_games = ActiveGameList::make(1, this);

  game_list = new GameList(this);
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
}

bool Engine::play_selected_game() {

  if (last_selected.invalid()) {
    return false;
  }

  auto game_path = game_list->at(last_selected);
  if (game_path.isEmpty()) {
    return false;
  }

  return play_game(game_path);
}

bool Engine::play_game(const QString& path) {

  if (active_games->maxed_out()) {
    return false;
  }

  auto* game = ActiveGame::make(this);

  if (game->open(path)) {
    active_games->add(game);
    return true;
  } else {
    return false;
  }
}

void Engine::handle_exit() {
  store_settings();
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
