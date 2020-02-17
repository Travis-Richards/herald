#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectManager;
class TableItemEditor;

/// Creates a new animation editor.
/// @param manager A pointer to the project manager.
/// @returns A new table item editor for animations.
ScopedPtr<TableItemEditor> make_animation_editor(ProjectManager* manager);

} // namespace tk

} // namespace herald
