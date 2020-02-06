#pragma once

#include <QObject>

class QSettings;
class QString;
class QStringList;

/// Contains information that assists in enumerating
/// and opening existing games on the user's system.
class GameList : public QObject {
  Q_OBJECT
public:
  /// Creates a new game list.
  /// @param parent A pointer to the parent object.
  GameList(QObject* parent);
  /// Releases memory allocated by the list.
  ~GameList();
  /// Adds a path to the game list.
  void add(const QString& path);
  /// Removes a game from the game list.
  /// @param index The index of the game to remove.
  void remove(int index);
  /// Gets an entry from the game list.
  /// @returns An entry from the game list.
  /// This string is a path to the directory
  /// containing the game.
  QString at(int index) const;
  /// Accesses the number of entries in the game list.
  int size() const;
  /// Loads the game list from settings.
  void load(QSettings& settings);
  /// Stores the game list into settings.
  void store(QSettings& settings) const;
private:
  /// A list of paths that point to games.
  QStringList* path_list;
};
