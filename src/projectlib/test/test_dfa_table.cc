// These are integration tests between parser,N FA, DFA, DFATable

#include "catch.hpp"

#include "../src/dfa.h"
#include "../src/dfa_table.h"
#include "../src/nfa.h"
#include "../src/parser.h"
#include "../src/plotter.h"

using namespace std;
using namespace regex_compiler;
using namespace dfa_table;

static DFATable dfa_table_from_regex(const char* regex) {
  auto p = Parser(regex);
  auto nfa = nfa::NFA(p.begin(), p.end());
  auto dfa = dfa::DFA(nfa);
  return DFATable{dfa};
}

TEST_CASE("Parse and build DFATable", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("");
}

TEST_CASE("Parse and build DFATable for /a/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("a");
  CHECK(dfa.accepts("a"));
  CHECK(!dfa.accepts("aa"));
  CHECK(!dfa.accepts("b"));
}

TEST_CASE("Parse and build DFATable for /a*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("a*");
  CHECK(dfa.accepts(""));
  CHECK(dfa.accepts("a"));
  CHECK(dfa.accepts("aa"));
  CHECK(!dfa.accepts("b"));
}

TEST_CASE("Parse and build DFATable for /ab/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("ab");
  CHECK(dfa.accepts("ab"));
  CHECK(!dfa.accepts("a"));
  CHECK(!dfa.accepts("b"));
}

TEST_CASE("Parse and build DFATable for /ab*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("ab*");
  CHECK(dfa.accepts("ab"));
  CHECK(dfa.accepts("abb"));
  CHECK(!dfa.accepts("aab"));
}

TEST_CASE("Parse and build DFATable for /a|b/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("a|b");
  CHECK(dfa.accepts("b"));
  CHECK(dfa.accepts("a"));
  CHECK(!dfa.accepts("aa"));
  CHECK(!dfa.accepts("ab"));
}

TEST_CASE("Parse and build DFATable for /a|b*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("a|b*");
  CHECK(dfa.accepts("a"));
  CHECK(dfa.accepts("b"));
  CHECK(dfa.accepts("bb"));
  CHECK(!dfa.accepts("aa"));
}

TEST_CASE("Parse and build DFATable for /(ac)|b*/", "[Parser, NFA, DFA]") {
  auto dfa = dfa_table_from_regex("(ac)|b*");
  CHECK(dfa.accepts("ac"));
  CHECK(dfa.accepts("b"));
  CHECK(dfa.accepts("bb"));
  CHECK(!dfa.accepts("ba"));
}

