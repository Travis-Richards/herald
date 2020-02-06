#pragma once

#include <QObject>

class ActiveGame;

/// The active game list is responsible
/// for containing each actively running game.
class ActiveGameList : public QObject {
  Q_OBJECT
public:
  /// Creates a new instance of the active game list.
  /// @param max The maximum number of active games to contain.
  /// @param parent A pointer to the parent object.
  static ActiveGameList* make(int max, QObject* parent);
  /// Constructs the base active game list.
  /// @param parent A pointer to the parent object.
  ActiveGameList(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~ActiveGameList() {}
  /// Adds a game to the active game list.
  /// @param game The game to add to the list.
  virtual void add(ActiveGame* game) = 0;
  /// Indicates if the maximum amount of games are active.
  /// @returns True if the list is maxed out, false otherwise.
  virtual bool maxed_out() const noexcept = 0;
};
