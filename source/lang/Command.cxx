#include "Command.h"

#include "ScopedPtr.h"

#include <string>

namespace herald {

namespace {

/// The base of most commands.
/// Contains a string to add data to.
class CommandBase : public Command {
  /// The command data.
  std::string data;
public:
  /// Accesses the command data.
  const char* get_data() const noexcept override {
    return data.c_str();
  }
  /// Accesses the size of the command data.
  std::size_t get_size() const noexcept override {
    return data.size();
  }
protected:
  /// Appends a string to the command data.
  /// @param str The string to append.
  void append(const char* str) {
    data += str;
  }
  /// Appends a single character to the command data.
  /// @param c The character to append.
  void append(char c) {
    data += c;
  }
  /// Appends a boolean value to the command data.
  /// @param value The value to append.
  void append(bool value) {
    data += value ? "true" : "false";
  }
  /// Appends a size value to the command data.
  /// @param value The value to append.
  void append(std::size_t value) {
    data += std::to_string(value);
  }
  /// Appends a floating point value to the command data.
  /// @param value The value to append.
  void append(double value) {
    data += std::to_string(value);
  }
};

/// A command with no operands.
class NullaryCommand final : public CommandBase {
public:
  /// Creates a command with no operands.
  /// @param name The name of the command.
  NullaryCommand(const char* name) {
    append(name);
    append('\n');
  }
};

/// A command for updating a controller axis.
class AxisUpdateCommand final : public CommandBase {
public:
  AxisUpdateCommand(std::size_t c, double x, double y) {
    append(c);
    append(' ');
    append(x);
    append(' ');
    append(y);
    append('\n');
  }
};

/// A command for updating a controller button state.
class ButtonUpdateCommand final : public CommandBase {
public:
  ButtonUpdateCommand(std::size_t c, std::size_t b, bool state) {
    append(c);
    append(' ');
    append(b);
    append(' ');
    append(state);
    append('\n');
  }
};

/// A null command, used mostly
/// as a placeholder.
class NullCommand final : public Command {
public:
  const char* get_data() const noexcept override {
    return "";
  }
  std::size_t get_size() const noexcept override {
    return 0;
  }
};

} // namespace

ScopedPtr<Command> Command::make_null() {
  return new NullCommand();
}

ScopedPtr<Command> Command::make_nullary(const char* name) {
  return new NullaryCommand(name);
}

ScopedPtr<Command> Command::make_axis_update(std::size_t controller, double x, double y) {
  return new AxisUpdateCommand(controller, x, y);
}

ScopedPtr<Command> Command::make_button_update(std::size_t controller, std::size_t button, bool state) {
  return new ButtonUpdateCommand(controller, button, state);
}

} // namespace herald
