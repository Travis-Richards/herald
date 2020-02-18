#include "Token.h"

#include <ostream>

#include <cstring>

namespace herald {

namespace protocol {

bool Token::has_data(const char* other_data) const noexcept {

  auto l = std::strlen(other_data);
  if (l != size) {
    return false;
  } else {
    return std::memcmp(data, other_data, size) == 0;
  }
}

std::ostream& operator << (std::ostream& output, const Token& t) {

  output << '\'';

  if (t.has_type(TokenType::Newline)) {
    output << "\\n";
  } else {
    for (std::size_t i = 0; i < t.get_size(); i++) {
      output << t.at(i);
    }
  }

  output << '\'';

  return output;
}

} // namespace protocol

} // namespace herald
