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
  /// Builds the main menu of the game.
  /// @param scene The scene to put the menu data into.
  /// @returns True on success, false on failure.
  virtual bool build_menu(Scene* scene) = 0;
  /// Builds a specified level.
  /// @returns True on success, false on failure.
  virtual bool build_level(int id, Scene* scene) = 0;
  /// Exits the game.
  virtual void exit() = 0;
  /// Gets the number of levels defined by the API.
  virtual int get_level_count() = 0;
signals:
  /// This signal is emitted when a symbol failed to load from the API implementation.
  /// This happens when, for example, a Java class can't be found.
  /// @param type The type of the symbol that is missing.
  /// @param name The name of the symbol that is missing.
  void symbol_not_found(SymbolType type, const char* name);
  /// This is emitted when an unknown error occurs from the API code.
  /// @param desc A description of the error.
  void unknown_error(const QString& desc);
};