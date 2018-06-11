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

TEST_CASE("A group consisting of one letter can be parsed", "[Parser]") {
  auto p = Parser("(a)");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Group>(*begin(p)));
}

TEST_CASE("An unclosed group should not parse", "[Parser]") {
  CHECK_THROWS(Parser("(a"));
}

TEST_CASE("A group consisting of two letters can be parsed", "[Parser]") {
  auto p = Parser("(ab)");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Group>(*begin(p)));
}

TEST_CASE("A group consisting of two groups can be parsed", "[Parser]") {
  auto p = Parser("((a)(b))");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Group>(*begin(p)));
  auto g = get<Group>(*begin(p));
  CHECK(g.pattern.size() == 2);
  CHECK_NOTHROW(get<Group>(g.pattern[0]));
  CHECK_NOTHROW(get<Group>(g.pattern[1]));
}

TEST_CASE("Two alternative single letter patterns can be parsed", "[Parser]") {
  auto p = Parser("a|b");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Alternative>(*begin(p)));
}

TEST_CASE("Alternative of two groups can be parsed", "[Parser]") {
  auto p = Parser("(ab)|(cd)");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Alternative>(*begin(p)));
}

TEST_CASE("Alternative can be followed by more pattern", "[Parser]") {
  auto p = Parser("(ab)|(cd)a");

  CHECK(p.size() == 2);
  CHECK_NOTHROW(get<Alternative>(*begin(p)));
}

TEST_CASE("Kleene closure of a single letter can be parsed", "[Parser]") {
  auto p = Parser("a*");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Kleene>(*begin(p)));
}

TEST_CASE("Kleene closure can be followed by more pattern", "[Parser]") {
  auto p = Parser("a*b");

  CHECK(p.size() == 2);
  CHECK_NOTHROW(get<Kleene>(*begin(p)));
}

TEST_CASE("Alternative of Kleene closure can be parsed", "[Parser]") {
  auto p = Parser("a*|b");

  CHECK(p.size() == 1);
  CHECK_NOTHROW(get<Alternative>(*begin(p)));
}
