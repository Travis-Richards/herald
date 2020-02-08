#pragma once

#include <QGraphicsRectItem>

class Background;
class TextureAnimationGroup;
class QSize;

/// The background view of a scene.
class BackgroundView : public QGraphicsRectItem {
public:
  /// Creates a full instance of the background view implementation.
  /// @param parent A pointer to the parent graphics item.
  static BackgroundView* make(QGraphicsItem* parent);
  /// Constructs the base of the background view.
  /// @param parent A pointer to the parent graphics item.
  BackgroundView(QGraphicsItem* parent) : QGraphicsRectItem(parent) {}
  /// Just a stub.
  virtual ~BackgroundView() {}
  /// Maps the background information into a viewable background.
  /// @param background The background data.
  /// @param animation list for the game.
  virtual void map(const Background& background, const TextureAnimationGroup& animations) = 0;
  /// Resizes the view of the background.
  /// @param size The size to adjust the background view to.
  virtual void resize_view(const QSize& size) = 0;
};
