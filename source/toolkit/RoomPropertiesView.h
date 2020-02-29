#pragma once

#include <QWidget>

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class Room;

/// A view of the current room properties.
class RoomPropertiesView : public QWidget {
  Q_OBJECT
public:
  /// Constructs a new instance of a room properties view.
  /// @param parent A pointer to the parent widget.
  static ScopedPtr<RoomPropertiesView> make(QWidget* parent);
  /// Just a stub.
  virtual ~RoomPropertiesView() {}
  /// Updates the room data in the properties view.
  /// @param room A pointer to the room to get the data from.
  virtual void update_room_data(const Room* room) = 0;
signals:
  /// This signal is emitted when the user changes the height of the room.
  /// @param height The height that the room has changed to.
  void height_changed(std::size_t height);
  /// This signal is emitted when the user changes the width of the room.
  /// @param width The width that the room has changed to.
  void width_changed(std::size_t width);
protected:
  /// Constructs the base of the properties view.
  /// @param parent A pointer to the parent widget.
  RoomPropertiesView(QWidget* parent) : QWidget(parent) {}
};

} // namespace tk

} // namespace herald
