#pragma once

class QString;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// The manager of the toolkit as a whole.
class Manager {
public:
  /// Creates a new manager instance.
  static ScopedPtr<Manager> make();
  /// Just a stub.
  virtual ~Manager() {}
  /// Opens a game project.
  /// @param game_path The game project to open.
  /// @returns True on success, false on failure.
  virtual bool open_project(const QString& game_path) = 0;
};

} // namespace tk

} // namespace herald
