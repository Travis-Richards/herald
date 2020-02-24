#pragma once

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectModel;

/// Used for editing textures.
/// The root widget for the "Textures" tab from the project view.
class TextureEditor {
public:
  /// Creates a new texture editor instance.
  /// @param model The project model containing the textures.
  /// @param parent A pointer to the parent widget.
  /// @returns A new texture editor instance.
  static ScopedPtr<TextureEditor> make(ProjectModel* model, QWidget* parent);
  /// Just a stub.
  virtual ~TextureEditor() {}
  /// Accesses a pointer to the root widget.
  virtual QWidget* get_widget() noexcept = 0;
};

} // namespace tk

} // namespace herald
