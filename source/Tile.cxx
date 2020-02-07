#include "Tile.h"

Tile::Tile(QObject* parent) : QObject(parent) {
  flags = 0;
  frame = 0;
  frame_offset = 0;
  texture = 0;
}

bool Tile::has_texture(int t) const noexcept {
  return texture == t;
}

int Tile::get_flags() const noexcept {
  return flags;
}

int Tile::get_frame() const noexcept {
  return frame + frame_offset;
}

int Tile::get_texture() const noexcept {
  return texture;
}

void Tile::set_flags(int f) noexcept {
  flags = f;
}

void Tile::set_frame(int index) noexcept {
  frame = index;
}

void Tile::set_frame_offset(int offset) noexcept {
  frame_offset = offset;
}

void Tile::set_texture(int index) noexcept {
  texture = index;
}
