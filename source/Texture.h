#pragma once

#include <QObject>

class QBrush;
class QPixmap;
class QSize;

/// This is the interface for a texture.
class Texture : public QObject {
public:
  /// Opens a new texture.
  /// @param path The path to the texture to open.
  /// @param parent A pointer to the parent object.
  /// @returns A new texture instance.
  static Texture* open(const QString& path, QObject* parent);
  /// Creates a texture from an existing pixmap.
  /// @param pixmap The pixmap to create the texture for.
  /// @param parent A pointer to the parent object.
  static Texture* make(const QPixmap& pixmap, QObject* parent);
  /// Constructs the base texture instance.
  Texture(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~Texture() {}
  /// Converts the texture into a brush.
  /// @param tile_size The size of the tile
  /// this brush is getting made for.
  /// @returns The brush for the texture.
  virtual QBrush as_brush(const QSize& tile_size) const = 0;
};
