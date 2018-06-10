#include "catch.hpp"

#include "../src/parser.h"

using namespace std;
using namespace regex_compiler;

TEST_CASE("The empty string can be parsed", "[Parser]") {
    auto p = Parser("");
}

TEST_CASE("A Parser behaves like a container") {
    auto p = Parser("");

    size_t s = p.size();
    Parser::iterator b = p.begin();
    Parser::iterator e = p.end();

    (void)s;
}

TEST_CASE("A single letter pattern can be parsed", "[Parser]") {
  auto p = Parser("a");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Literal>(*begin(p)));
  CHECK(get<Literal>(*begin(p)).pattern == 'a');
}

TEST_CASE("A two letter pattern can be parsed", "[Parser]") {
  auto p = Parser("ab");

  CHECK(p.size() == 2);
  CHECK_NOTHROW(get<Literal>(*begin(p)));
  CHECK(get<Literal>(*begin(p)).pattern == 'a');
}
