#pragma once

class QProcess;

namespace herald {

template <typename T>
class ScopedPtr;

class Model;

namespace tk {

class Console;

/// This class represents the process
/// that is started to run the game.
class GameProcess {
public:
  /// Creates a new game process instance.
  /// @param model A pointer to the game model for the process to control.
  /// @param console A console to log the errors to.
  /// @returns A pointer to a new game process instance.
  static ScopedPtr<GameProcess> make(Model* model, Console* console);
  /// Just a stub.
  virtual ~GameProcess() {}
  /// Accesses a pointer to the Qt process object.
  virtual QProcess* get_process() noexcept = 0;
  /// Waits to acknowledge the process exit
  /// status or kills the process if it's still running.
  /// @param timeout_ms The number of milliseconds
  /// to give the process to exit.
  virtual void waitOrKill(int timeout_ms = 1000) = 0;
};

} // namespace tk

} // namespace herald
