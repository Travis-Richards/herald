#pragma once

class QWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProjectModel;

/// Creates a new room editor widget.
/// @param model A pointer to the project data model.
/// @param parent A pointer to the parent widget.
/// @returns A widget containing the roomt editor.
ScopedPtr<QWidget> make_room_editor(ProjectModel* model, QWidget* parent);

} // namespace tk

} // namespace herald
