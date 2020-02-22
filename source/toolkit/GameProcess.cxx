#include "GameProcess.h"

#include <herald/ScopedPtr.h>

#include <herald/Model.h>

#include "Console.h"

#include <QProcess>

namespace herald {

namespace tk {

namespace {

/// An implementation of the game process interface.
class GameProcessImpl final : public GameProcess {
  /// The Qt implementation of a process object.
  QProcess process;
  /// A pointer to the model that the game
  /// process will modify.
  Model* model;
  /// A pointer to the console to log errors to.
  Console* console;
public:
  /// Constructs a new instance of a game process.
  /// @param m The model for the process to modify.
  /// @param c A console to log errors to.
  GameProcessImpl(Model* m, Console* c)
    : model(m), console(c) {

    auto read_stdout_functor = [this]() { read_stdout(); };
    auto read_stderr_functor = [this]() { read_stderr(); };

    QObject::connect(&process, &QProcess::readyReadStandardOutput, read_stdout_functor);
    QObject::connect(&process, &QProcess::readyReadStandardError, read_stderr_functor);
  }
  /// Accesses a pointer to the Qt process object.
  QProcess* get_process() noexcept override {
    return &process;
  }
  /// Waits on the process or kills it if it's still running.
  /// @param timeout_ms The number of milliseconds to give the
  /// process to exit.
  void waitOrKill(int timeout_ms) override {
    if (!process.waitForFinished(timeout_ms)) {
      process.kill();
      process.waitForFinished(timeout_ms);
    }
  }
protected:
  /// Reads the process' standard output.
  void read_stdout() {

    process.setReadChannel(QProcess::StandardOutput);

    //console->log_stdout(process.readLine());
  }
  /// Reads the process' standard error output.
  void read_stderr() {

    process.setReadChannel(QProcess::StandardError);

    console->log_stderr(process.readLine());
  }
};

} // namespace

ScopedPtr<GameProcess> GameProcess::make(Model* model, Console* console) {
  return new GameProcessImpl(model, console);
}

} // namespace tk

} // namespace herald
