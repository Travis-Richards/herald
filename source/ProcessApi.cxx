#include "ProcessApi.h"

#include "Api.h"
#include "LineBuffer.h"
#include "MenuBuilder.h"
#include "RoomBuilder.h"
#include "Scene.h"

#include "lang/Interpreter.h"
#include "lang/Writer.h"

#include <QProcess>
#include <QString>
#include <QStringList>

namespace {

/// An implementation of an API
/// using an external process and redirected IO.
class ProcessApi final : public Api {
  /// The process to emit the engine commands.
  QProcess process;
  /// The line buffer for the standard output.
  LineBuffer* out_line_buffer;
  /// The line buffer for standard error output.
  LineBuffer* err_line_buffer;
  /// A pointer to the response interpreter.
  Interpreter* interpreter;
public:
  /// Constructs an instance of the process API.
  /// @param parent A pointer to the parent object.
  ProcessApi(QObject* parent)
    : Api(parent),
      out_line_buffer(nullptr),
      err_line_buffer(nullptr),
      interpreter(nullptr) {

    out_line_buffer = LineBuffer::from_process_stdout(process, this);
    err_line_buffer = LineBuffer::from_process_stderr(process, this);

    connect(out_line_buffer, &LineBuffer::line, this, &ProcessApi::handle_line);
    connect(err_line_buffer, &LineBuffer::line, this, &Api::error_log);
  }
  /// Builds a room.
  /// @returns True on success, false on failure.
  bool build_room(Scene* scene) override {

    scene->clear();

    if (interpreter) {
      delete interpreter;
    }

    interpreter = make_room_builder(scene, this);

    process.write(Writer::build_room());

    return true;
  }
  /// Sends a message to the process that the engine is exiting
  /// and then waits for the process to exit.
  void exit() override {

    process.write(Writer::exit());

    const int timeout_ms = 5000;

    if (!process.waitForFinished(timeout_ms)) {
      process.kill();
      process.waitForFinished(timeout_ms);
    }
  }
  /// Initializes the game in the process' working directory.
  /// @returns True on success, false on failure.
  bool init_game() override {
    return true;
  }
  /// Assigns the working directory of the process.
  /// This should only be called before the process is started.
  /// @param pwd The path to place the process into.
  void set_working_directory(const QString& pwd) {
    process.setWorkingDirectory(pwd);
  }
  /// Starts the process.
  /// @param program The path to the program to start.
  /// @param args The arguments to pass to the program.
  /// @returns True on success, false on failure.
  bool start(const QString& program, const QStringList& args) {
    process.start(program, args);
    return true;
  }
protected slots:
  /// Handles a line from the games standard output.
  /// If no interpreter is active, then the line is ignored.
  /// @param line The line emitted from the process.
  void handle_line(const QString& line) {

    if (!interpreter) {
      return;
    }

    interpreter->interpret_text(line);

    delete interpreter;

    interpreter = nullptr;
  }
};

} // namespace

Api* make_process_api(const QString& program,
                      const QString& path,
                      const QStringList& args,
                      QObject* parent) {

  auto process_api = new ProcessApi(parent);
  process_api->set_working_directory(path);
  process_api->start(program, args);
  return process_api;
}
