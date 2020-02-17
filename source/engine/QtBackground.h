#pragma once

#include <herald/Background.h>

class QGraphicsItem;
class QSize;

namespace herald {

template <typename T>
class ScopedPtr;

/// A Qt implementation of a background.
class QtBackground : public Background {
public:
  /// Creates a new instance of a Qt background.
  /// @param parent A pointer to the parent object.
  /// @returns A new Qt background instance.
  static ScopedPtr<QtBackground> make(QGraphicsItem* parent);
  /// Just a stub.
  virtual ~QtBackground() {}
  /// Accesses the graphical item held by the background.
  virtual QGraphicsItem* get_graphics_item() = 0;
  /// Handles a window resize event.
  /// @param size The window size to scale to.
  virtual void handle_resize(const QSize& size) = 0;
};

} // namespace herald
