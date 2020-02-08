#pragma once

#include <QObject>

enum class Button : int;

class QString;
class Scene;

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
  /// Builds a room from the game.
  /// @param scene The scene to put the menu data into.
  /// @returns True on success, false on failure.
  virtual bool build_room(Scene* scene) = 0;
  /// Exits the game.
  virtual void exit() = 0;
  /// Fills the scene with objects.
  /// @param scene The scene to fill with objects.
  /// @returns True on success, false on failure.
  virtual bool fill_objects(Scene* scene) = 0;
  /// Assigns the background to the scene.
  /// @param scene The scene to assign the background of.
  /// @returns True on success, false on failure.
  virtual bool set_background(Scene* scene) = 0;
public slots:
  /// Updates the axis for the default player.
  void update_def_axis(double x, double y) {
    update_axis(0, x, y);
  }
  /// Updates the button state for the default player.
  void update_def_button(Button button, bool state) {
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
  virtual void update_button(int controller, Button button, bool state) = 0;
signals:
  /// This signal is emitted when the background
  /// finished being assigned.
  void background_set();
  /// This signal is emitted when the game
  /// sends an error message. It should not
  /// be treated as a fatal error but should
  /// be logged somewhere.
  void error_log(const QString& line);
  /// This signal is emitted when a room
  /// is done being built.
  void room_built();
  /// This signal is emitted when the
  /// object map is done being built.
  void object_map_built();
};
