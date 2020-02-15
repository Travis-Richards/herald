#pragma once

namespace herald {

class Model;

} // namespace herald

class Interpreter;
class QObject;

/// Creates an interpreter
/// for putting objects onto a model.
/// @param model The model to put the objects into.
/// @param parent A pointer to the parent object to assign the interpreter.
/// @returns a pointer to a new interpreter instance.
Interpreter* make_object_table_builder(herald::Model* model, QObject* parent);
