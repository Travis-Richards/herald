#include "RoomToolMediator.h"

#include <herald/ScopedPtr.h>

#include "Project.h"

namespace herald {

namespace tk {

namespace {

/// The implementation of the stamp tool.
class StampToolImpl final : public StampTool {
  /// A pointer to the project.
  const Project* project;
  /// The index of the texture currently being used.
  std::size_t texture_index;
public:
  /// Constructs a new instance of the stamp tool.
  /// @param p A pointer to the project.
  StampToolImpl(const Project* p) : project(p), texture_index(0) {

    auto* texture_table = project->access_texture_table();

    connect(texture_table, &TextureTable::renamed, this, &StampToolImpl::on_renamed);
    connect(texture_table, &TextureTable::added,   this, &StampToolImpl::on_added);
    connect(texture_table, &TextureTable::removed, this, &StampToolImpl::on_removed);
  }
  /// Accesses the current texture data.
  QByteArray get_texture_data() const override {
    return project->access_texture_table()->get_data(texture_index);
  }
  /// Accesses the name of the texture currently used.
  QString get_texture_name() const override {
    return project->access_texture_table()->get_name(texture_index);
  }
  /// Lists the available textures for the stamp tool.
  /// @returns A list of texture names for the tool.
  QStringList list_textures() const override {

    const auto* texture_table = project->access_texture_table();

    auto count = texture_table->size();

    QStringList texture_names;

    for (std::size_t i = 0; i < count; i++) {
      texture_names << texture_table->get_name(i);
    }

    return texture_names;
  }
  /// Sets the currently used texture.
  /// @param index The index of the texture to use.
  void set_current_texture(std::size_t index) override {
    texture_index = index;
  }
protected:
  /// Called when a texture is added.
  void on_added(std::size_t) {
    emit texture_table_updated();
  }
  /// Called when a texture is removed.
  void on_removed(const QString&) {
    emit texture_table_updated();
  }
  /// Called when a texture is renamed.
  void on_renamed(std::size_t) {
    emit texture_table_updated();
  }
};

/// This is the implementation of the room tool model.
class RoomToolMediatorImpl final : public RoomToolMediator {
  /// The ID of the current tool.
  RoomToolID current_tool_id;
  /// The eraser tool instance.
  EraserTool eraser_tool;
  /// The stamp tool instance.
  StampToolImpl stamp_tool;
  /// A placeholder tool instance.
  NullRoomTool null_tool;
public:
  /// Constructs a new room tool model.
  /// @param project_model The project data to read tool data from.
  RoomToolMediatorImpl(const Project* project, QObject* parent)
    : RoomToolMediator(parent),
      stamp_tool(project) {

    current_tool_id = RoomToolID::None;
  }
  /// Sets the current tool being used.
  /// @param id The ID of the tool to use.
  void set_current_tool(RoomToolID id) override {

    if (id != current_tool_id) {
      emit tool_changed(id);
    }

    current_tool_id = id;
  }
  /// Accesses a pointer to the current room tool.
  RoomTool* get_current_tool() override {

    switch (current_tool_id) {
      case RoomToolID::None:
        break;
      case RoomToolID::Eraser:
        return &eraser_tool;
      case RoomToolID::Move:
        break;
      case RoomToolID::Rotate:
        break;
      case RoomToolID::Stamp:
        return &stamp_tool;
    }

    return &null_tool;
  }
  /// Accesses a pointer to the stamp tool.
  StampTool* get_stamp_tool() override {
    return &stamp_tool;
  }
};

} // namespace

ScopedPtr<RoomToolMediator> RoomToolMediator::make(const Project* project, QObject* parent) {
  return new RoomToolMediatorImpl(project, parent);
}

} // namespace tk

} // namespace herald
