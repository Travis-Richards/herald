#pragma once

#include <QObject>

/// This class represents a running game instance.
class ActiveGame : public QObject {
  Q_OBJECT
public:
  /// Creates a new active game instance.
  /// @param parent A pointer to the parent object.
  static ActiveGame* make(QObject* parent);
  /// Constructs the base active game instance.
  ActiveGame(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~ActiveGame() {}
  /// Closes the game, if it's running.
  virtual void close() = 0;
  /// Opens a game at a specified path.
  /// @param path The path to the game to open.
  /// @returns True on success, false on failure.
  virtual bool open(const QString& path) = 0;
signals:
  /// Emitted when the game is shutting down.
  void closing(ActiveGame* game);
};
