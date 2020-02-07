#pragma once

#include <QObject>

/// Enumerates the available
/// buttons to the controller.
enum class Button : int {
  /// Usage defined by the game.
  A,
  /// Usage defined by the game.
  B,
  /// Usage defined by the game.
  X,
  /// Usage defined by the game.
  Y
};

/// Converts the button to an ID value.
/// @param button The button to convert.
/// @returns An integer value indicating the button ID.
inline int button_id(Button button) noexcept {

  switch (button) {
    case Button::A:
      return 0;
    case Button::B:
      return 1;
    case Button::X:
      return 3;
    case Button::Y:
      return 4;
  }

  return 0;
}

/// Used for controlling player actions.
class Controller : public QObject {
  Q_OBJECT
public:
  /// Constructs a controller base instance.
  /// @param parent A pointer to the parent object.
  Controller(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~Controller() {}
signals:
  /// This signal is emitted when the axis is updated.
  /// @param x The new X value (from -1 to 1)
  /// A value of 1 means complete "right" and
  /// a value of -1 means complete "left".
  /// @param y The new Y value (from -1 to 1)
  /// A value of 1 means complete "up"
  /// and a value of -1 means complete "down".
  void update_axis(double x, double y);
  /// This signal is emitted when a button changes state.
  /// @param button The button that changed state.
  /// @param state The state that the button changed to.
  void update_button(Button button, bool state);
};
