#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectManager;
class TableItemEditor;

/// Creates a texture editor.
/// @param manager A pointer to the project manager.
/// @returns A new table item editor for textures.
ScopedPtr<TableItemEditor> make_texture_editor(ProjectManager* manager);

} // namespace tk

} // namespace herald
