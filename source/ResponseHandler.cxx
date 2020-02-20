#include "ResponseHandler.h"

#include <herald/Model.h>
#include <herald/Object.h>
#include <herald/ObjectTable.h>
#include <herald/ScopedPtr.h>

#include <herald/protocol/Parser.h>
#include <herald/protocol/ParseTree.h>

#include "Interpreter.h"

namespace herald {

namespace {

/// Used for handling an arbitrary response from the game.
class ResponseHandler final : public Interpreter, public protocol::Visitor {
  /// The model for the response to modify.
  Model* model;
public:
  /// Constructs the response handler.
  /// @param m The model to modify by the response.
  /// @param parent A pointer to the parent object.
  ResponseHandler(Model* m, QObject* parent) : Interpreter(parent), model(m) { }
  /// Interprets the text sent from the game.
  /// In this case, the response can do a large variety of modifications.
  /// @param parser A reference to the parser to check the response with.
  /// @returns True on success, false on failure.
  bool interpret(herald::protocol::Parser& parser) override {

    auto node = parser.parse_any();
    if (!node) {
      return false;
    }

    node->accept(*this);

    return true;
  }
protected:
  /// Assigns an object within the model a different action.
  /// @param set_action_stmt A reference to the statement
  /// from the parse tree that contains the information
  /// regarding the object to modify and the action to assign to it.
  void visit(const protocol::SetActionStmt& set_action_stmt) override {

    int action_id = 0;
    int object_id = 0;

    auto action_id_node = set_action_stmt.get_action_id();
    auto object_id_node = set_action_stmt.get_object_id();

    if (!check(action_id_node)
     || !check(object_id_node)) {
      return;
    }

    if (!object_id_node.to_signed_value(object_id)
     || !action_id_node.to_signed_value(action_id)) {
      return;
    }

    auto* object_table = model->get_object_table();

    auto* object = object_table->at((std::size_t) object_id);
    if (!object) {
      return;
    }

    object->set_action_index((std::size_t) action_id);
  }
  /// Does nothing.
  void visit(const protocol::Integer&) override { }
  /// Does nothing.
  void visit(const protocol::Size&) override {}
  /// Does nothing.
  void visit(const protocol::Matrix&) override {}
};

} // namespace

} // namespace herald

Interpreter* make_response_handler(herald::Model* model, QObject* parent) {
  return new herald::ResponseHandler(model, parent);
}
