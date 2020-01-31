#pragma once

#include <QObject>

class QString;

/// Used to store information of a game.
class GameInfo : public QObject {
public:
  /// Opens the information for a game at a specified path.
  /// @param parent A pointer to the parent object.
  /// @param path The path to the game containing the info.
  /// The info is stored in a file called "info.txt", which
  /// is automatically appended to this path.
  /// @returns If the file was opened properly, then a pointer
  /// to the game info instance is returned. If the file doesn't
  /// exist or an error occurs when opening it, a null pointer is returned.
  static GameInfo* open(const QString& path, QObject* parent = nullptr);
  /// Constructs the base of the game info class.
  /// @param parent A pointer to the parent object.
  GameInfo(QObject* parent);
  /// Just a stub.
  ~GameInfo() {}
  /// Gets the API used by the game.
  QString get_api() const;
  /// Gets the author or maker of the game.
  QString get_author() const;
  /// Gets the title of the game.
  QString get_title() const;
private:
  /// The API language used to run the game.
  QString* api;
  /// The author or maker of the game.
  QString* author;
  /// The title of the game.
  QString* title;
};
