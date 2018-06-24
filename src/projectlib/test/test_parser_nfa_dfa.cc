// These are integration tests between parser,NFA, and DFA
// These build on the tests from test_parser_nfa.cc

#include "catch.hpp"

#include "../src/dfa.h"
#include "../src/nfa.h"
#include "../src/parser.h"
#include "../src/plotter.h"

using namespace std;
using namespace regex_compiler;
using namespace dfa;

static DFA dfa_from_regex(const char* regex) {
  auto p = Parser(regex);
  auto nfa = nfa::NFA(p.begin(), p.end());
  return DFA(nfa);
}

TEST_CASE("Parse and build DFA", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("");
}

TEST_CASE("Parse and build DFA for /a/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("a");
  Plotter(dfa, "/tmp/dfa_a.dot");
  CHECK(dfa.accepts("a"));
  CHECK(!dfa.accepts("aa"));
  CHECK(!dfa.accepts("b"));
}

TEST_CASE("Parse and build DFA for /a*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("a*");
  Plotter(dfa, "/tmp/dfa_a_kleene.dot");
  CHECK(dfa.accepts(""));
  CHECK(dfa.accepts("a"));
  CHECK(dfa.accepts("aa"));
  CHECK(!dfa.accepts("b"));
}

TEST_CASE("Parse and build DFA for /ab/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("ab");
  Plotter(dfa, "/tmp/dfa_ab.dot");
  CHECK(dfa.accepts("ab"));
  CHECK(!dfa.accepts("a"));
  CHECK(!dfa.accepts("b"));
}

TEST_CASE("Parse and build DFA for /ab*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("ab*");
  Plotter(dfa, "/tmp/dfa_ab_kleene_.dot");
  CHECK(dfa.accepts("ab"));
  CHECK(dfa.accepts("abb"));
  CHECK(!dfa.accepts("aab"));
}

TEST_CASE("Parse and build DFA for /a|b/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("a|b");
  Plotter(dfa, "/tmp/dfa_a_alternate_b.dot");
  CHECK(dfa.accepts("b"));
  CHECK(dfa.accepts("a"));
  CHECK(!dfa.accepts("aa"));
  CHECK(!dfa.accepts("ab"));
}

TEST_CASE("Parse and build DFA for /a|b*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("a|b*");
  Plotter(dfa, "/tmp/dfa_a_alternate_b_kleene.dot");
  CHECK(dfa.accepts("a"));
  CHECK(dfa.accepts("b"));
  CHECK(dfa.accepts("bb"));
  CHECK(!dfa.accepts("aa"));
}

TEST_CASE("Parse and build DFA for /(ac)|b*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_from_regex("(ac)|b*");
  Plotter(dfa, "/tmp/dfa_open_a_c_close_alternate_b_kleene.dot");
  CHECK(dfa.accepts("ac"));
  CHECK(dfa.accepts("b"));
  CHECK(dfa.accepts("bb"));
  CHECK(!dfa.accepts("ba"));
}
