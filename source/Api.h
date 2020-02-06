#pragma once

#include <QObject>

class QString;
class Scene;

/// Enumerates the types of symbols
/// that can be loaded from an API.
/// This is used for error reporting.
enum class SymbolType : int {
  Class,
  Function
};

/// Accesses a human-readable name for @p type.
const char* to_string(SymbolType type) noexcept;

/// This class is responsible for abstracting
/// the API in which the games are designed in.
/// Currently, there is only a Java API, but there
/// may be a Python or Javascript API in the future.
class Api : public QObject {
  Q_OBJECT
public:
  /// Constructs the API.
  /// @param parent A pointer to the parent object.
  Api(QObject* parent = nullptr);
  virtual ~Api();
  /// Initializes the game object.
  virtual bool init_game() = 0;
  /// Builds a room from the game.
  /// @param scene The scene to put the menu data into.
  /// @returns True on success, false on failure.
  virtual bool build_room(Scene* scene) = 0;
  /// Exits the game.
  virtual void exit() = 0;
signals:
  /// This signal is emitted when the game
  /// sends an error message. It should not
  /// be treated as a fatal error but should
  /// be logged somewhere.
  void error_log(const QString& line);
};
