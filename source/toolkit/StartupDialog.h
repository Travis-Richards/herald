#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// The dialog used to startup the program.
class StartupDialog {
public:
  /// Creates a new instance of the startup dialog.
  static ScopedPtr<StartupDialog> make();
  /// Just a stub.
  virtual ~StartupDialog() {}
  /// Shows the dialog window.
  /// By default, the dialog is hidden.
  virtual void show() = 0;
};

} // namespace tk

} // namespace herald
