#pragma once

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectManager;

/// The widget used for editing code.
class CodeEditor {
public:
  /// Creates a new instance of the code editor.
  /// @param manager A pointer to the project manager.
  /// @param parent A pointer to the parent widget.
  /// @returns A new code editor instance.
  static ScopedPtr<CodeEditor> make(ProjectManager* manager, QWidget* parent);
  /// Just a stub.
  virtual ~CodeEditor() {}
  /// Gets a pointer to the widget containing the code editor.
  virtual QWidget* get_widget() noexcept = 0;
};

} // namespace tk

} // namespace herald
