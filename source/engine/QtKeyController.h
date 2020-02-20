#pragma once

#include <herald/Controller.h>

class QKeyEvent;

namespace herald {

/// A game controller based on keyboard input.
class QtKeyController : public Controller {
public:
  /// Constructs the key controller instance.
  /// @param o A pointer to the controller state observer.
  QtKeyController(Controller::Observer* o) : Controller(o) {}
  /// Handles a key press event.
  /// Internally, this is used to generate controller signals.
  void handle_key_press(const QKeyEvent* event) {
    map(event, true);
  }
  /// Handles a key release event.
  /// Internally, this is used to generate controller signals.
  void handle_key_release(const QKeyEvent* event) {
    map(event, false);
  }
protected:
  /// Maps a key event to a button or axis state.
  /// @param event The keyboard event to handle.
  /// @param state The state of the key.
  void map(const QKeyEvent* event, bool state);
};

} // namespace herald
