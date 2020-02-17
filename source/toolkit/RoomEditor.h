#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectManager;
class TableItemEditor;

/// Creates a new room editor.
/// @param manager A pointer to the project manager.
/// @returns A new table item editor for rooms.
ScopedPtr<TableItemEditor> make_room_editor(ProjectManager* manager);

} // namespace tk

} // namespace herald
