#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectManager;
class TableItemEditor;

/// Creates a new action editor.
/// @param manager A pointer to the project manager.
/// @returns A new table item editor for actions.
ScopedPtr<TableItemEditor> make_action_editor(ProjectManager* manager);

} // namespace tk

} // namespace herald
