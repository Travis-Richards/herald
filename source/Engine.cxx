#include "Engine.h"

#include "ActiveGame.h"
#include "ActiveGameList.h"
#include "GameList.h"
#include "SelectionIndex.h"

#include <QSettings>
#include <QString>
#include <QStringList>

namespace {

/// Implements the engine interface.
class EngineImpl final : public Engine {
  /// The actively running games.
  ActiveGameList* active_games;
  /// The list of available games.
  GameList* game_list;
  /// The last selected game.
  SelectionIndex last_selected;
public:
  /// Constructs an instance of the engine implementation.
  /// @param parent A pointer to the parent object.
  EngineImpl(QObject* parent) : Engine(parent) {
    active_games = ActiveGameList::make(1, this);
    game_list = new GameList(this);
  }
  /// Loads user-specific settings.
  /// @returns True on success, false on failure.
  bool load_settings() override {

    QSettings settings;

    if (!game_list->load(settings)) {
      return false;
    }

    emit updated_game_list(*game_list);

    return true;
  }
  /// Stores user settings.
  /// @returns True on success, false on failure.
  bool save_settings() const override {
    QSettings settings;
    return game_list->store(settings);
  }
  /// Selects a game by its index.
  /// @param index The index of the game within the game list.
  void select_game(int index) override {
    if (index < game_list->size()) {
      last_selected = index;
    } else {
      last_selected = -1;
    }
  }
  /// Deletes the last selected game.
  void delete_game() override {

    if (last_selected.invalid()) {
      return;
    }

    game_list->remove(last_selected);

    last_selected = -1;

    emit updated_game_list(*game_list);
  }
  /// Opens a game by its path.
  /// @param path The path of the game to open.
  void open_game(const QString& path) override {
    game_list->add(path);
    emit updated_game_list(*game_list);
  }
  /// Plays the last selected game.
  /// @returns True on success, false on failure.
  bool play_selected_game() override;
  /// Plays a game by its specified path.
  /// @param path The path of the game to play.
  /// @returns True on success, false on failure.
  bool play_game(const QString& path);
  /// Handles the case of the game
  /// engine exiting.
  void handle_exit() override {
    save_settings();
  }
};

bool EngineImpl::play_selected_game() {

  if (last_selected.invalid()) {
    return false;
  }

  auto game_path = game_list->at(last_selected);
  if (game_path.isEmpty()) {
    return false;
  }

  return play_game(game_path);
}

bool EngineImpl::play_game(const QString& path) {

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

} // namespace

Engine* Engine::make(QObject* parent) {
  return new EngineImpl(parent);
}
