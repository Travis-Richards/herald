#include "ProcessApi.h"

#include "Api.h"
#include "BackgroundModifier.h"
#include "Controller.h"
#include "Interpreter.h"
#include "LineBuffer.h"
#include "MenuBuilder.h"
#include "ObjectFiller.h"
#include "RoomBuilder.h"
#include "Scene.h"
#include "Writer.h"

#include "lang/SyntaxChecker.h"

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

    if (interpreter) {
      delete interpreter;
    }

    interpreter = make_room_builder(scene, this);

    connect(interpreter, &Interpreter::finished, this, &Api::room_built);

    connect(interpreter, &Interpreter::error, this, &ProcessApi::handle_syntax_error);

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
  /// Fills the current room with objects.
  /// @param scene The scene to fill.
  /// @returns True on success, false on failure.
  bool fill_objects(Scene* scene) override {

    if (interpreter) {
      delete interpreter;
    }

    interpreter = make_object_filler(scene, this);

    connect(interpreter, &Interpreter::finished, this, &Api::object_map_built);

    connect(interpreter, &Interpreter::error, this, &ProcessApi::handle_syntax_error);

    process.write(Writer::fill_objects());

    return true;
  }
  /// Assigns the background of the scene.
  /// @param scene The scene to assign the background of.
  /// @returns True on success, false on failure.
  bool set_background(Scene* scene) override {

    if (interpreter) {
      delete interpreter;
    }

    interpreter = make_background_modifier(scene, this);

    connect(interpreter, &Interpreter::finished, this, &Api::background_set);

    connect(interpreter, &Interpreter::error, this, &ProcessApi::handle_syntax_error);

    process.write(Writer::set_background());

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
  /// Notifies the process in the change of an axis value.
  /// @param controller The index of the controller.
  /// @param x The X value of the axis.
  /// @param y The Y value of the axis.
  void update_axis(int controller, double x, double y) override {
    process.write(Writer::update_axis(controller, x, y));
  }
  /// Notifies the process of a change in button state.
  /// @param controller The index of the controller.
  /// @param button The button that changed state.
  /// @param state The new state of the button.
  void update_button(int controller, Button button, bool state) override {
    process.write(Writer::update_button(controller, button_id(button), state));
  }
protected slots:
  /// Handles a line from the games standard output.
  /// If no interpreter is active, then the line is ignored.
  /// @param line The line emitted from the process.
  void handle_line(const QString& line) {
    if (interpreter) {
      interpreter->interpret_text(line);
    }
  }
  /// Handles a syntax error from the response.
  void handle_syntax_error(const SyntaxError& error) {
    emit error_log(QString(error.get_description()));
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
