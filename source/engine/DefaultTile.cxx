#include "DefaultTile.h"

namespace herald {

namespace {

class DefaultTileImpl final : public DefaultTile {
  std::vector<Index> animation_queue;
  std::vector<Index> frame_queue;
public:
  void queue_animation(Index animation) override {
    animation_queue.push_back(animation);
  }
  void queue_frame(Index frame) override {
    frame_queue.push_back(frame);
  }
};

} // namespace

ScopedPtr<DefaultTile> DefaultTile::make() {
  return new DefaultTileImpl;
}

} // namespace herald
