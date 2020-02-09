#include "Lexer.h"

#include <QString>

Lexer::Lexer(const QString& input_, QObject* parent) : QObject(parent), input(input_), pos(0) {

}

bool Lexer::scan() {

  if (at_end()) {
    return false;
  }

  auto first = peek(0);
  if (first == '\r') {
    return complete_newline();
  } else if (first == '\n') {
    return complete(TokenType::Newline, 1);
  } else if ((first == ' ') || (first == '\t')) {
    return complete_space();
  } else if ((first == '\'') || (first == '"')) {
    return complete_string_literal(first);
  } else if ((first == '-') && peek(1).isNumber()) {
    return complete_number();
  }

  if (first.isLetter() || (first == '_')) {
    return complete_identifier();
  }

  if (first.isDigit()) {
    return complete_number();
  }

  if (first != 0) {
    return complete(TokenType::Invalid, 1);
  } else {
    return false;
  }
}

bool Lexer::at_end() {
  return pos >= input.size();
}

bool Lexer::out_of_bounds(int offset) const noexcept {
  auto index = pos + offset;
  return (index < 0) || (index >= input.size());
}

int Lexer::remaining() const noexcept {
  auto r = input.size() - pos;
  return (r < 0) ? 0 : r;
}

QChar Lexer::peek(int offset) const {
  return out_of_bounds(offset) ? 0 : input[pos + offset];
}

void Lexer::next(int count) noexcept {
  pos += count;
}

bool Lexer::complete_newline() {
  auto line_feed = peek(1);
  if (line_feed == '\n') {
    return complete(TokenType::Newline, 2);
  } else {
    return complete(TokenType::Newline, 1);
  }
}

bool Lexer::complete_number() {

  for (int i = 1; !out_of_bounds(i); i++) {
    auto ch = peek(i);
    if (!ch.isDigit()
     && (ch != '.')
     && (ch != '-')
     && (ch != 'e')) {
      return complete(TokenType::Number, i);
    }
  }

  return complete(TokenType::Number, remaining());
}

bool Lexer::complete_identifier() {

  for (int i = 1; !out_of_bounds(i); i++) {
    auto ch = peek(i);
    if (!ch.isLetterOrNumber() && (ch != '_')) {
      return complete(TokenType::Identifier, i);
    }
  }

  return complete(TokenType::Identifier, remaining());
}

bool Lexer::complete_space() {

  for (int i = 1; !out_of_bounds(i); i++) {
    auto ch = peek(i);
    if ((ch != ' ') && (ch != '\t')) {
      return complete(TokenType::Space, i);
    }
  }

  return complete(TokenType::Space, remaining());
}

bool Lexer::complete_string_literal(const QChar& quote) {

  for (int i = 1; !out_of_bounds(i); i++) {
    auto c = peek(i);
    if (c == quote) {
      emit token_found(TokenType::StringLiteral, QStringRef(&input, pos + 1, i - 1));
      next(i + 1);
      return true;
    } else if (c == '\\') {
      i++;
    }
  }

  return complete(TokenType::UnterminatedStringLiteral, remaining());
}

bool Lexer::complete(TokenType type, int size) {
  emit token_found(type, QStringRef(&input, pos, size));
  next(size);
  return true;
}
