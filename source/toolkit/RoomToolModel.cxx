#include "RoomToolModel.h"

#include <herald/ScopedPtr.h>

#include "Project.h"

namespace herald {

namespace tk {

namespace {

/// The implementation of the stamp tool.
class StampToolImpl final : public StampTool {
  /// A pointer to the project.
  const Project* project;
public:
  /// Constructs a new instance of the stamp tool.
  /// @param p A pointer to the project.
  StampToolImpl(const Project* p) : project(p) {

  }
  /// Accesses the current texture data.
  QByteArray get_texture_data() const override {
    return QByteArray();
  }
  /// Accesses the name of the texture currently used.
  QString get_texture_name() const override {
    return QString();
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
};

/// This is the implementation of the room tool model.
class RoomToolModelImpl final : public RoomToolModel {
  /// The ID of the current tool.
  RoomToolID current_tool_id;
  /// The stamp tool data model.
  StampToolImpl stamp_tool;
  /// A placeholder tool instance.
  NullRoomTool null_tool;
public:
  /// Constructs a new room tool model.
  /// @param project_model The project data to read tool data from.
  RoomToolModelImpl(const Project* project, QObject* parent)
    : RoomToolModel(parent),
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
        break;
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

ScopedPtr<RoomToolModel> RoomToolModel::make(const Project* project, QObject* parent) {
  return new RoomToolModelImpl(project, parent);
}

} // namespace tk

} // namespace herald
