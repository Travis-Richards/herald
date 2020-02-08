#pragma once

class Interpreter;
class Scene;
class QObject;

/// Creates a background modifier instance.
/// @param scene The scene to modify the background of.
/// @param parent A pointer to the parent object.
/// @returns A new interpreter instance pointing to the background modifier.
Interpreter* make_background_modifier(Scene* scene, QObject* parent);
