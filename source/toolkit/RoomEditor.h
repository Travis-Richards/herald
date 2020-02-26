#pragma once

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class Project;

/// Creates a new room editor widget.
/// @param project A pointer to the project data.
/// @param parent A pointer to the parent widget.
/// @returns A widget containing the roomt editor.
ScopedPtr<QWidget> make_room_editor(Project* project, QWidget* parent);

} // namespace tk

} // namespace herald
