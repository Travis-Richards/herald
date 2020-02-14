#include "QtEngine.h"
#include "QtTarget.h"

#include "Animation.h"
#include "AnimationTable.h"
#include "Model.h"
#include "Index.h"
#include "Room.h"
#include "ScopedPtr.h"
#include "TextureTable.h"
#include "Tile.h"

#include <QApplication>
#include <QGraphicsView>

namespace {

void setup(herald::Engine& engine) {

  using namespace herald;

  auto* model = engine.get_model();

  auto* animations = model->get_animation_table();
  animations->add(Animation::make_single_frame(0));
  animations->add(Animation::make_single_frame(0));
  animations->add(Animation::make_single_frame(0));

  auto* textures = model->get_texture_table();
  textures->open("/home/tay10r/Pictures/medieval-city-2d-tileset/PNG/land/land_1.png");
  textures->open("/home/tay10r/Pictures/medieval-city-2d-tileset/PNG/land/land_2.png");
  textures->open("/home/tay10r/Pictures/medieval-city-2d-tileset/PNG/land/land_3.png");

  auto* room = model->get_room();
  room->resize(5, 5);
  room->at(0, 0)->queue_animation(1);
  room->at(4, 4)->queue_animation(2);
}

} // namespace

int main(int argc, char** argv) {

  QApplication app(argc, argv);

  auto target = herald::QtTarget::make(nullptr);

  target->get_graphics_view()->show();

  auto engine = herald::QtEngine::make(std::move(target));

  setup(*engine);

  engine->advance(1000);

  return app.exec();
}
