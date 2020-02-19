#pragma once

namespace herald {

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
class Controller {
public:
  /// A class used for observing
  /// changes to the controller state.
  class Observer {
  public:
    /// Just a stub.
    virtual ~Observer() {}
    /// Called when an axis update occurs.
    /// @param x The new X value.
    /// @param y The new Y value.
    virtual void axis_update(double x, double y) = 0;
    /// Called when a button changes state.
    /// @param button The button that changed state.
    /// @param state The state that the button changed to.
    /// True indicates the button is pressed, false
    /// means that it is not pressed.
    virtual void button_update(Button button, bool state) = 0;
  };
  /// Constructs an instance of the controller.
  /// @param o A pointer to the observer.
  constexpr Controller(Observer* o = nullptr) noexcept : observer(o) {}
  /// Just a stub.
  virtual ~Controller() {}
  /// Assigns a new observer to the controller.
  inline void set_observer(Observer* o) noexcept {
    observer = o;
  }
protected:
  /// Notifies the observer of an axis update.
  void update_axis(double x, double y) {
    if (observer) {
      observer->axis_update(x, y);
    }
  }
  /// Notifies the observer of a button update.
  void update_button(Button button, bool state) {
    if (observer) {
      observer->button_update(button, state);
    }
  }
private:
  /// A pointer to the controller observer.
  Observer* observer;
};

} // namespace herald
