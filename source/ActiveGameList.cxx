#include "ActiveGameList.h"

#include "ActiveGame.h"

#include <QList>

namespace {

/// This is the implementation of the active game list interface.
class ActiveGameListImpl final : public ActiveGameList {
  /// The max number of games to contain.
  int max;
  /// The list of active games.
  QList<ActiveGame*> active_games;
public:
  /// Constructs a new instance of the active game list.
  /// @param m The max number of contains to contain.
  /// @param parent A pointer to the parent object.
  ActiveGameListImpl(int m, QObject* parent) : ActiveGameList(parent), max(m) {}
  /// Adds a game to the active game list.
  /// @param game The game to add to the list.
  void add(ActiveGame* game) override {

    active_games.push_back(game);

    connect(game, &ActiveGame::closing, this, &ActiveGameListImpl::close);
  }
  /// Indicates if the list has reached its max.
  bool maxed_out() const noexcept override {
    return active_games.size() >= max;
  }
protected slots:
  /// Closes a specified game.
  /// @param game The game to close.
  void close(ActiveGame* game) {
    for (auto it = active_games.begin(); it != active_games.end(); it++) {
      if (*it == game) {
        game->close();
        active_games.erase(it);
        break;
      }
    }
  }
};

} // namespace

ActiveGameList* ActiveGameList::make(int max, QObject* parent) {
  return new ActiveGameListImpl(max, parent);
}
