#pragma once

#include <cstddef>

namespace herald {

template <typename T>
class ScopedPtr;

namespace protocol {

/// The base of a command.
class Command {
public:
  /// Creates an axis update command.
  /// @param controller The index of the controller whose axis is updated.
  /// @param x The new X value.
  /// @param y The new Y value.
  /// @returns A new command instance.
  static ScopedPtr<Command> make_axis_update(std::size_t controller, double x, double y);
  /// Creates a button state update command.
  /// @param controller The index of the controller whose button is updated.
  /// @param button The ID of the button that changed states.
  /// @param state The new state of the button.
  /// @returns A new command instance.
  static ScopedPtr<Command> make_button_update(std::size_t controller, std::size_t button, bool state);
  /// Creates a null command.
  /// This kind of command has no data
  /// and is mostly used as a placeholder.
  static ScopedPtr<Command> make_null();
  /// Creates a command with no operands.
  /// @param data The command data to assign.
  /// @returns A new command instance.
  static ScopedPtr<Command> make_nullary(const char* data);
  /// Just a stub.
  virtual ~Command() {}
  /// Accesses the command data.
  /// @returns A null-terminated string containing the command data.
  virtual const char* get_data() const noexcept = 0;
  /// Accesses the size of the command data.
  /// @returns The size, in terms of bytes, of the command data.
  virtual std::size_t get_size() const noexcept = 0;
};

} // namespace protocol

} // namespace herald
