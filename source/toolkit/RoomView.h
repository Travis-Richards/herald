#pragma once

#include <cstddef>

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class RoomModel;
class RoomToolModel;

/// Used to display a room within the room editor.
class RoomView {
public:
  /// Creates a new room view instance.
  /// @param room A pointer to the room model to view.
  /// @param room_tools A pointer to the room tool model.
  /// @param parent A pointer to the parent widget.
  /// @returns A new room view instance.
  static ScopedPtr<RoomView> make(RoomModel* room, RoomToolModel* room_tools, QWidget* parent);
  /// Just a stub.
  virtual ~RoomView() {}
  /// Accesses a pointer to the widget for the room view.
  virtual QWidget* get_widget() noexcept = 0;
  /// Hides the grid of the room.
  virtual void hide_grid() = 0;
  /// Shows the grid of the room.
  virtual void show_grid() = 0;
};

} // namespace tk

} // namespace herald
