#pragma once

#include <QGraphicsItemGroup>

class Room;
class TextureAnimationGroup;
class QSize;

/// A view of the current room being played.
class RoomView : public QGraphicsItemGroup {
public:
  /// Creates a new instance of a room view.
  /// @param parent A pointer to the parent graphics item.
  /// @returns A new room view instance.
  static RoomView* make(QGraphicsItem* parent);
  /// Constructs the base of the room view.
  RoomView(QGraphicsItem* parent) : QGraphicsItemGroup(parent) {}
  /// Just a stub.
  virtual ~RoomView() {}
  /// Resizes the room view.
  /// @param w The new width of the room.
  /// @param h The new height of the room.
  virtual void resize(int w, int h) = 0;
  /// Resizes the view of the room.
  /// This is called when the window is resized,
  /// so that the size and position of each
  /// rectangle can be adjusted.
  virtual void resize_view(const QSize& size) = 0;
  /// Frames the frames from animations onto the room view.
  /// @param room The room containing the tile data.
  /// @param textures The texture animations for each of the tiles.
  virtual void map(const Room* room, const TextureAnimationGroup* textures) = 0;
};
