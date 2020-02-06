#pragma once

#include <QObject>

class Api;
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
  /// Gets the author or maker of the game.
  virtual QString get_author() const = 0;
  /// Gets the error description.
  virtual QString get_error() const = 0;
  /// Gets the title of the game.
  virtual QString get_title() const = 0;
  /// Creates an API based on the game info.
  /// @param path The path to run the API from.
  /// @param parent A pointer to the parent object for the API.
  /// @returns A new API instance on success, null on failure.
  virtual Api* make_api(const QString& path, QObject* parent) const = 0;
};
