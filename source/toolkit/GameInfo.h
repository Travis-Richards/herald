#pragma once

class QString;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// Used to contain basic
/// game metadata.
class GameInfo {
public:
  /// Opens a game information instance.
  /// @param path The path of the file to open.
  static ScopedPtr<GameInfo> open(const QString& path);
  /// Just a stub.
  virtual ~GameInfo() {}
  /// Accesses the API used by the game.
  virtual QString api() const = 0;
  /// Accesses the title of the game.
  virtual QString title() const = 0;
  /// Accesses the path to the game.
  virtual QString path() const = 0;
};

} // namespace tk

} // namespace herald
