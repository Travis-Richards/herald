#include "ParseTree.h"

#include "Interpreter.h"

#include <QRectF>

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

DrawBoxStmt::DrawBoxStmt(const QPointF& a,
                           const QPointF& b,
                           int texture_id_) {
  point_a = new QPointF(a);
  point_b = new QPointF(b);
  texture_id = texture_id_;
}

DrawBoxStmt::~DrawBoxStmt() {
  delete point_a;
  delete point_b;
}

void DrawBoxStmt::accept(Interpreter& interpreter) const {
  interpreter.interpret(*this);
}
