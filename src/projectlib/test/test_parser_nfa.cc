// These are integration tests between parser and NFA

#include "catch.hpp"

#include "../src/parser.h"
#include "../src/nfa.h"

using namespace std;
using namespace regex_compiler;

TEST_CASE("Parse and build NFA", "[Parser, NFA]") {
  auto p = Parser("");
  auto nfa = NFA(p.begin(), p.end());
}
