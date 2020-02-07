#pragma once

class QByteArray;

/// Used for writing strings that may be
/// sent to a game as a command.
class Writer {
public:
  /// Creates an "add player" request.
  static QByteArray add_player();
  /// Creates a "build room" request.
  static QByteArray build_room();
  /// Creates an exit request.
  static QByteArray exit();
  /// Updates an axis value.
  /// @param c The controller index.
  /// @param x The new X value.
  /// @param y The new Y value.
  static QByteArray update_axis(int c, double x, double y);
  /// Updates a button state.
  /// @param c The controller index.
  /// @param b The button ID.
  /// @param state The state of the button.
  static QByteArray update_button(int c, int b, bool state);
};
