#pragma once

#include <QObject>

class QString;

/// Used to store information of a game.
class GameInfo : public QObject {
public:
  /// Opens a game information instace from a path.
  /// @param prefix The prefix path of containing the "info.json" file.
  /// @param parent A pointer to the parent object.
  /// @returns A new game info instance.
  static GameInfo* open(const QString& path, QObject* parent = nullptr);
  /// Constructs the base of the game info class.
  /// @param parent A pointer to the parent object.
  GameInfo(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~GameInfo() {}
  /// Indicates whether or not an error occurred opening the info.
  virtual bool has_error() const = 0;
  /// Gets the API used by the game.
  virtual QString get_api() const = 0;
  /// Gets the author or maker of the game.
  virtual QString get_author() const = 0;
  /// Gets the error description.
  virtual QString get_error() const = 0;
  /// Gets the title of the game.
  virtual QString get_title() const = 0;
};
