#include "ProcessApi.h"

#include <herald/ScopedPtr.h>

#include "Api.h"
#include "BackgroundModifier.h"
#include "Controller.h"
#include "Interpreter.h"
#include "LineBuffer.h"
#include "MenuBuilder.h"
#include "ObjectMapBuilder.h"
#include "RoomBuilder.h"
#include "WorkQueue.h"
#include "Writer.h"

#include "protocol/Command.h"
#include "protocol/SyntaxChecker.h"

#include <QProcess>
#include <QString>
#include <QStringList>

using namespace herald;

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
  /// The queue of work items.
  ScopedPtr<WorkQueue> work_queue;
  /// Whether or not the command to exit
  /// the game was requested.
  bool exit_requested;
public:
  /// Constructs an instance of the process API.
  /// @param parent A pointer to the parent object.
  ProcessApi(QObject* parent)
    : Api(parent),
      out_line_buffer(nullptr),
      err_line_buffer(nullptr),
      work_queue(nullptr) {

    exit_requested = false;

    work_queue = WorkQueue::make();

    out_line_buffer = LineBuffer::from_process_stdout(process, this);
    err_line_buffer = LineBuffer::from_process_stderr(process, this);

    connect(out_line_buffer, &LineBuffer::line, this, &ProcessApi::handle_line);
    connect(err_line_buffer, &LineBuffer::line, this, &Api::error_logged);

    connect(&process, &QProcess::errorOccurred, this, &ProcessApi::handle_process_error);

    connect(&process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &ProcessApi::handle_finished);
  }
  /// Sends a message to the process that the engine is exiting
  /// and then waits for the process to exit.
  void exit() override {

    exit_requested = true;

    process.write(Writer::exit());

    const int timeout_ms = 5000;

    if (!process.waitForFinished(timeout_ms)) {
      process.kill();
      process.waitForFinished(timeout_ms);
    }
  }
  /// Starts up the game.
  /// @param model The model to add the game data into.
  /// @returns True on success, false on failure.
  bool start(Model* model) override {
    add_work_item(Command::make_nullary("set_background"), make_background_modifier(model, this));
    add_work_item(Command::make_nullary("build_room"), make_room_builder(model, this));
    add_work_item(Command::make_nullary("build_object_map"), make_object_table_builder(model, this));
    return handle_work_item();
  }
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
    add_work_item(Command::make_axis_update(controller, x, y), nullptr);
  }
  /// Notifies the process of a change in button state.
  /// @param controller The index of the controller.
  /// @param button The button that changed state.
  /// @param state The new state of the button.
  void update_button(int controller, Button button, bool state) override {
    add_work_item(Command::make_button_update(controller, button_id(button), state), nullptr);
  }
protected slots:
  /// Handles the finishing signal emitted from the process.
  /// @param exit_code The exit code returned by the process.
  /// @param status The exit status of the process.
  void handle_finished(int exit_code, QProcess::ExitStatus status) {
    if (status == QProcess::CrashExit) {
      emit error_occurred(QString("Game process crashed."));
    } else if (!exit_requested) {
      emit error_occurred(QString("Game process exited prematurely (exit code: ")
                        + QString(exit_code)
                        + QString(")"));
    }
  }
  /// Handles a line from the games standard output.
  /// If no interpreter is active, then the line is ignored.
  /// @param line The line emitted from the process.
  void handle_line(const QString& line) {

    if (work_queue->empty()) {
      return;
    }

    work_queue->get_current_interpreter().interpret_text(line);

    work_queue->pop();

    handle_work_item();
  }
  /// Handles a syntax error from the response.
  void handle_syntax_error(const SyntaxError& error) {
    emit error_occurred(QString(error.get_description()));
  }
  /// Handles an error emitted by the process.
  /// @param error A code describing the error.
  void handle_process_error(QProcess::ProcessError error) {
    QString desc;
    switch (error) {
      case QProcess::FailedToStart:
        desc = "Failed to start process '" + process.program() + "'.";
        break;
      case QProcess::Crashed:
        desc = "Game process crashed.";
        break;
      case QProcess::Timedout:
        desc = "Game startup timed out.";
        break;
      case QProcess::WriteError:
        desc = "Failed to write data to game.";
        break;
      case QProcess::ReadError:
        desc = "Failed to read data from game.";
        break;
      case QProcess::UnknownError:
        desc = "Unknown process error occurred.";
        break;
    }
    emit error_occurred(desc);
  }
protected:
  /// Adds an item to the work queue.
  /// @param cmd The command to add.
  /// @param interpreter The interpreter to handle the response.
  void add_work_item(ScopedPtr<Command>&& cmd, Interpreter* interpreter) {

    if (interpreter) {
      connect(interpreter, &Interpreter::error, this, &ProcessApi::handle_syntax_error);
    }

    work_queue->add(std::move(cmd), interpreter);
  }
  /// Sends a command to the process.
  /// @param command The command to send.
  /// @returns True on success, false on failure.
  bool send_command(const Command& command) {
    auto write_count = process.write(command.get_data(), command.get_size());
    if (write_count < 0) {
      return false;
    } else {
      return ((std::size_t) write_count) == command.get_size();
    }
  }
  /// Handles the current item in the work queue.
  /// @returns True on success, false on failure.
  bool handle_work_item() {

    if (work_queue->empty()) {
      return true;
    }

    return send_command(work_queue->get_current_command());
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
