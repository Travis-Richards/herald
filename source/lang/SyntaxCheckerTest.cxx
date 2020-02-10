#include <gtest/gtest.h>

#include "ParseTree.h"
#include "ScopedPtr.h"
#include "SyntaxChecker.h"

TEST(SyntaxChecker, MissingIntegerValue) {

  auto syntax_errors = SyntaxErrorList::make();

  auto syntax_checker = make_syntax_checker(syntax_errors.get());

  parse_tree::Integer value(nullptr, nullptr);

  value.accept(*syntax_checker);

  ASSERT_EQ(syntax_errors->size(), 1);

  const auto* error = syntax_errors->at(0);

  EXPECT_EQ(error->get_id(), SyntaxErrorID::MissingIntegerValue);
}
