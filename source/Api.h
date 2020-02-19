#pragma once

#include <QObject>

namespace herald {

enum class Button : int;

class Model;

} // namespace herald

class QString;

/// This class is responsible for abstracting
/// the API in which the games are designed in.
/// Currently, there is only a Java API, but there
/// may be a Python or Javascript API in the future.
class Api : public QObject {
  Q_OBJECT
public:
  /// Constructs the API.
  /// @param parent A pointer to the parent object.
  Api(QObject* parent = nullptr) : QObject(parent) {}
  /// Just a stub.
  virtual ~Api() {}
  /// Starts the game.
  /// @param model The game model to be modified.
  /// @returns True on success, false on failure.
  virtual bool start(herald::Model* model) = 0;
  /// Exits the game.
  virtual void exit() = 0;
public slots:
  /// Updates the axis for the default player.
  void update_def_axis(double x, double y) {
    update_axis(0, x, y);
  }
  /// Updates the button state for the default player.
  void update_def_button(herald::Button button, bool state) {
    update_button(0, button, state);
  }
  /// Updates an axis value for a controller.
  /// @param controller The index of the controller.
  /// @param x The X value of the axis.
  /// @param y The Y value of the axis.
  virtual void update_axis(int controller, double x, double y) = 0;
  /// Updates a button state from a controller.
  /// @param controller The index of the controller sending the update.
  /// @param button The button that changed state.
  /// @param state The new state of the button.
  virtual void update_button(int controller, herald::Button button, bool state) = 0;
signals:
  /// This signal is emitted when the background
  /// finished being assigned.
  void background_set();
  /// This signal is emitted when a runtime
  /// error occurs with the game. Typically
  /// this signal should interrupt game execution.
  void error_occurred(const QString& line);
  /// This signal is emitted when the game
  /// logs an error message. This signal should
  /// not interrupt game execution.
  void error_logged(const QString& line);
  /// This signal is emitted when a room
  /// is done being built.
  void room_built();
  /// This signal is emitted when the
  /// object map is done being built.
  void object_map_built();
};
