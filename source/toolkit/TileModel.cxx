#include "TileModel.h"

#include "ProjectModel.h"
#include "RoomModel.h"

namespace herald {

namespace tk {

TileModel::TileModel(RoomModel* m, std::size_t x_, std::size_t y_) : QObject(m) {
  model = m;
  x = x_;
  y = y_;
}

TileModel::TileModel(const TileModel& other) : TileModel(other.model, other.x, other.y) {

}

QString TileModel::get_texture() const {

  const auto* tile = model->access_tile(x, y);
  if (!tile) {
    return QString();
  }

  return tile->get_texture();
}

bool TileModel::is_valid() const {
  return model->access_tile(x, y) != nullptr;
}

void TileModel::set_texture(const QString& name) {
  auto* tile = model->modify_tile(x, y);
  // This function should never return
  // null, but we'll check just in case.
  if (tile) {
    tile->set_texture(name);
    emit changed();
  }
}

} // namespace tk

} // namespace herald
