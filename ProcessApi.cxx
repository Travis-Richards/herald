#include "ProcessApi.h"

#include "Api.h"
#include "LineBuffer.h"
#include "MenuBuilder.h"
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
  LineBuffer* line_buffer;
  /// A pointer to the response interpreter.
  Interpreter* interpreter;
public:
  /// Constructs an instance of the process API.
  /// @param parent A pointer to the parent object.
  ProcessApi(QObject* parent) : Api(parent), line_buffer(nullptr), interpreter(nullptr) {

    line_buffer = LineBuffer::from_process_stdout(process, this);

    connect(line_buffer, &LineBuffer::line, this, &ProcessApi::handle_line);
  }
  /// Builds the main menu.
  /// @returns True on success, false on failure.
  bool build_menu(Scene* scene) override {

    scene->clear();

    if (interpreter) {
      delete interpreter;
    }

    interpreter = make_menu_builder(scene, this);

    interpreter->set_root_path(process.workingDirectory());

    process.write(Writer::build_menu());

    return true;
  }
  /// Builds a level specified by @p id.
  /// @param id The ID of the level to build.
  /// @param scene A pointer of the scene to
  /// put the level data into.
  /// @returns True on success, false on failure.
  bool build_level(int id, Scene* scene) override {
    (void)id;
    (void)scene;
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
  /// Gets the number of playable levels.
  /// @returns The number of playable levels.
  int get_level_count() override {
    return 0;
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
    if (interpreter) {
      interpreter->interpret_line(line);
      if (interpreter->done()) {
        delete interpreter;
        interpreter = nullptr;
      }
    }
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
