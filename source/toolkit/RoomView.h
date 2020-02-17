#pragma once

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// Used to display a room within the room editor.
class RoomView {
public:
  /// Creates a new room view instance.
  /// @param parent A pointer to the parent widget.
  /// @returns A new room view instance.
  static ScopedPtr<RoomView> make(QWidget* parent);
  /// Just a stub.
  virtual ~RoomView() {}
  /// Accesses a pointer to the widget for the room view.
  virtual QWidget* get_widget() noexcept = 0;
};

} // namespace tk

} // namespace herald
