#pragma once

#include <QObject>

class QBrush;
class QSize;

/// This is the interface for a texture.
class Texture : public QObject {
public:
  /// Opens a new texture.
  /// @param path The path to the texture to open.
  /// @param parent A pointer to the parent object.
  /// @returns A new texture instance.
  static Texture* open(const QString& path, QObject* parent);
  /// Constructs the base texture instance.
  Texture(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~Texture() {}
  /// Converts the texture into a brush.
  /// @param tile_size The size of the tile
  /// this brush is getting made for.
  /// @returns The brush for the texture.
  virtual QBrush as_brush(const QSize& tile_size) = 0;
};
