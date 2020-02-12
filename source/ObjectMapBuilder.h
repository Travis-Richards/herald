#pragma once

class Interpreter;
class Scene;
class QObject;

/// Creates an interpreter
/// for putting objects onto a scene.
/// @param scene The scene to put the objects into.
/// @param parent A pointer to the parent object to assign the interpreter.
/// @returns a pointer to a new interpreter instance.
Interpreter* make_object_map_builder(Scene* scene, QObject* parent);
