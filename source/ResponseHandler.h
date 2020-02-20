#pragma once

namespace herald {

class Model;

} // namespace herald

class Interpreter;
class QObject;

/// Creates an interpreter for handling
/// arbitrary responses.
/// @param model The model to be modified from the response.
/// @param parent A pointer to the parent object.
/// @returns A new interpreter instance that handles arbitrary responses.
Interpreter* make_response_handler(herald::Model* model, QObject* parent);
