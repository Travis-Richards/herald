#pragma once

namespace herald {

class Model;

} // namespace herald

class Interpreter;
class QObject;

/// Creates a menu builder instance.
/// @param model The model to put the menu onto.
/// @param parent A pointer to a parent object.
/// This parameter may be null, in which case
/// the interpreter returned must be deleted manually.
/// @returns A pointer to an interpreter that builds the menu.
Interpreter* make_menu_builder(herald::Model* model, QObject* parent);
