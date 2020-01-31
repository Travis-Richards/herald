#include "Api.h"

const char* to_string(SymbolType type) noexcept {
  switch (type) {
    case SymbolType::Class:
      return "Class";
    case SymbolType::Function:
      return "Function";
  }
  return "";
}

Api::Api(QObject* parent) : QObject(parent) {
}

Api::~Api() {

}
