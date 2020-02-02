#include "ParseTree.h"

#include "Interpreter.h"

void BackgroundStmt::accept(Interpreter& interpreter) const {
  interpreter.set_background(texture_id);
}

void ColorTextureDecl::accept(Interpreter& interpreter) const {
  interpreter.interpret(*this);
}

void FinishStmt::accept(Interpreter& interpreter) const {
  interpreter.finish();
}

void ImageTextureDecl::accept(Interpreter& interpreter) const {
  interpreter.interpret(*this);
}

void PolygonObjectStmt::accept(Interpreter& interpreter) const {
  interpreter.interpret(*this);
}
