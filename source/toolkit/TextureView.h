#pragma once

class QString;
class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// Used for displaying a texture.
class TextureView {
public:
  /// Creates a new texture widget instance.
  /// @param parent A pointer to the parent widget.
  /// @returns A new texture widget instance.
  static ScopedPtr<TextureView> make(QWidget* parent);
  /// Just a stub.
  virtual ~TextureView() {}
  /// Accesses a pointer to the widget
  /// being used to display the texture.
  virtual QWidget* get_widget() noexcept = 0;
  /// Opens a texture to be displayed by the widget.
  /// @param filename The name of the file to open.
  /// @returns True on success, false on failure.
  virtual bool open_texture(const QString& filename) = 0;
};

} // namespace tk

} // namespace herald
