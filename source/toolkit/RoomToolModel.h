#pragma once

#include <QObject>

class QByteArray;
class QString;
class QStringList;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class Project;

/// Identifies the several types of room tools.
enum class RoomToolID : int {
  /// A placeholder for when no tool
  /// is currently selected.
  None,
  /// The eraser. This is used
  /// to remove content from a tile.
  Eraser,
  /// The move tool, used for rearranging tiles.
  Move,
  /// Used for rotating tiles.
  Rotate,
  /// The tile stamp. This is used
  /// to put artwork onto tiles.
  Stamp
};

class NullRoomTool;
class StampTool;

/// Used for accessing tool-specific information.
class RoomToolVisitor {
public:
  /// Just a stub.
  virtual ~RoomToolVisitor() {}
  /// Visits a null tool.
  /// In most cases, nothing needs to be done here.
  virtual void visit(const NullRoomTool&) { }
  /// Visits the stamp tool.
  /// @param stamp_tool The stamp tool to visit.
  virtual void visit(const StampTool& stamp_tool) = 0;
};

/// The base class of a room tool.
class RoomTool {
public:
  /// Just a stub.
  virtual ~RoomTool() {}
  /// Accepts a room tool visitor.
  /// @param visitor The visitor to accept.
  virtual void accept(RoomToolVisitor& visitor) const = 0;
};

/// This is either a room tool with
/// no associated data or a placeholder
/// until a room tool is selected.
class NullRoomTool final : public RoomTool {
public:
  /// Accepts a visitor.
  void accept(RoomToolVisitor& visitor) const override {
    visitor.visit(*this);
  }
};

/// The tile stamp tool.
class StampTool : public RoomTool {
public:
  /// Just a stub.
  virtual ~StampTool() {}
  /// Accepts a room tool visitor.
  /// @param visitor The visitor to accept.
  void accept(RoomToolVisitor& visitor) const override {
    visitor.visit(*this);
  }
  /// Gets the data of the texture that
  /// the stamp tool is applying.
  virtual QByteArray get_texture_data() const = 0;
  /// Gets the name of the texture that
  /// the stamp tool is applying.
  virtual QString get_texture_name() const = 0;
  /// Lists the available textures for the stamp tool.
  /// @returns A list of available textures.
  virtual QStringList list_textures() const = 0;
  /// Sets the index of the texture currently used texture.
  /// @param index The index of the texture to use.
  virtual void set_current_texture(std::size_t index) = 0;
};

/// This is the data model for the room tools.
class RoomToolModel : public QObject {
  Q_OBJECT
public:
  /// Creates a new room tool model instance.
  /// @param project A pointer to the project data.
  /// This is used to access texture and animation info for the room tools.
  /// @param parent A pointer to the parent object.
  static ScopedPtr<RoomToolModel> make(const Project* project, QObject* parent);
  /// Sets the current room tool being used.
  /// @param id The ID of the tool to assign.
  virtual void set_current_tool(RoomToolID id) = 0;
  /// Accesses the current room tool.
  /// @returns A pointer to the current room tool.
  virtual RoomTool* get_current_tool() = 0;
  /// Gets a pointer to the stamp tool.
  /// @returns A pointer to the stamp tool.
  virtual StampTool* get_stamp_tool() = 0;
signals:
  /// This signal is emitted whenever a tool gets selected.
  /// @param id The ID of the selected tool.
  void tool_changed(RoomToolID id);
protected:
  /// Constructs teh base of the room tool model.
  /// @param parent A pointer to the parent object.
  RoomToolModel(QObject* parent) : QObject(parent) {}
};

} // namespace tk

} // namespace herald
