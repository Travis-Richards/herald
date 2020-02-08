#pragma once

#include <QObject>

/// Represents a single object within a room.
/// This is not related, in any way, to the idea
/// of an object in object oriented programming or
/// in the Qt object model.
class Object : public QObject {
  /// The action the object
  /// is currently performing.
  int action;
  /// The X coordinate of the object's location.
  double x;
  /// The Y coordinate of the object's location.
  double y;
public:
  /// Constructs the base object.
  /// @param parent A pointer to the parent Qt object.
  Object(int a, int x_, int y_, QObject* parent)
    : QObject(parent),
      action(a),
      x(x_),
      y(y_) {}
};
