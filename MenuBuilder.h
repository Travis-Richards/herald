#pragma once

class Interpreter;
class Scene;
class QObject;

/// Creates a menu builder instance.
/// @param scene The scene that the menu will be rendered to.
/// @param parent A pointer to a parent object.
/// This parameter may be null, in which case
/// the interpreter returned must be deleted manually.
/// @returns A pointer to an interpreter that builds the menu.
Interpreter* make_menu_builder(Scene* scene, QObject* parent);
