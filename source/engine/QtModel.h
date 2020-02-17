#pragma once

#include <herald/Model.h>

#include <cstddef>

class QGraphicsScene;
class QSize;

namespace herald {

/// The Qt implementation of the model.
class QtModel : public Model {
public:
  /// Creates a new instance of the Qt model.
  static ScopedPtr<QtModel> make();
  /// Just a stub.
  virtual ~QtModel() {}
  /// Accesses a pointer to the graphics scene.
  /// @returns A pointer to the graphics scene.
  virtual QGraphicsScene* get_scene() = 0;
  /// Moves the items in the model
  /// forward in time, by a certain number of milliseconds.
  /// @param value The number of milliseconds to move forward by.
  virtual void advance(std::size_t delta_ms) = 0;
  /// Resizes the view of the model.
  /// This mainly for handling window resize events.
  /// @param size The size to scale everything to.
  virtual void resize(const QSize& size) = 0;
};

} // namespace herald
