#pragma once

#include "Controller.h"

class QKeyEvent;

/// A game controller based on keyboard input.
class KeyController : public Controller {
public:
  /// Constructs the key controller instance.
  /// @param parent A pointer to the parent object.
  KeyController(QObject* parent) : Controller(parent) {}
  /// Handles a key press event.
  /// Internally, this is used to generate controller signals.
  void handle_key_press(const QKeyEvent* event) {
    map(event, true);
  }
  /// Handles a key release event.
  /// Internally, this is used to generate controller signals.
  void handle_key_release(const QKeyEvent* event) {
    map(event, true);
  }
protected:
  /// Maps a key event to a button or axis state.
  /// @param event The keyboard event to handle.
  /// @param state The state of the key.
  void map(const QKeyEvent* event, bool state);
};
