// These are integration tests between parser and NFA

#include "catch.hpp"

#include "../src/nfa.h"
#include "../src/parser.h"
#include "../src/plotter.h"

using namespace std;
using namespace regex_compiler;

TEST_CASE("Parse and build NFA", "[Parser, NFA]") {
  auto p = Parser("");
  auto nfa = NFA(p.begin(), p.end());
}

TEST_CASE("Parse and build NFA for /a/", "[Parser, NFA]") {
  auto p = Parser("a");
  auto nfa = NFA(p.begin(), p.end());
  Plotter(nfa, "/tmp/a.dot");
}

TEST_CASE("Parse and build NFA for /ab/", "[Parser, NFA]") {
  auto p = Parser("ab");
  auto nfa = NFA(p.begin(), p.end());
  Plotter(nfa, "/tmp/ab.dot");
}

TEST_CASE("Parse and build NFA for /ab*/", "[Parser, NFA]") {
  auto p = Parser("ab*");
  auto nfa = NFA(p.begin(), p.end());
  Plotter(nfa, "/tmp/ab_kleene_.dot");
}

TEST_CASE("Parse and build NFA for /a|b*/", "[Parser, NFA]") {
  auto p = Parser("a|b*");
  auto nfa = NFA(p.begin(), p.end());
}

TEST_CASE("Parse and build NFA for /(ac)|b*/", "[Parser, NFA]") {
  auto p = Parser("(ac)|b*");
  auto nfa = NFA(p.begin(), p.end());
}
