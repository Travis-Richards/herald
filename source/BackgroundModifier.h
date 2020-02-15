#pragma once

namespace herald {

class Model;

} // namespace herald

class Interpreter;
class QObject;

/// Creates a background modifier instance.
/// @param model The model containing the background to modify.
/// @param parent A pointer to the parent object.
/// @returns A new interpreter instance pointing to the background modifier.
Interpreter* make_background_modifier(herald::Model* model, QObject* parent);
