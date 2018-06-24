// These are integration tests between parser and NFA

#include "catch.hpp"

#include "../src/nfa.h"
#include "../src/parser.h"
#include "../src/plotter.h"

using namespace std;
using namespace regex_compiler;
using namespace nfa;

TEST_CASE("Parse and build NFA", "[Parser, NFA]") {
  auto p = Parser("");
  auto nfa = NFA(p.begin(), p.end());
}

TEST_CASE("Parse and build NFA for /a/", "[Parser, NFA]") {
  auto p = Parser("a");
  auto nfa = NFA(p.begin(), p.end());
  Plot(nfa, "/tmp/a.dot");
}

TEST_CASE("Parse and build NFA for /a*/", "[Parser, NFA]") {
  auto p = Parser("a*");
  auto nfa = NFA(p.begin(), p.end());
  Plot(nfa, "/tmp/a_kleene.dot");
}

TEST_CASE("Parse and build NFA for /ab/", "[Parser, NFA]") {
  auto p = Parser("ab");
  auto nfa = NFA(p.begin(), p.end());
  Plot(nfa, "/tmp/ab.dot");
}

TEST_CASE("Parse and build NFA for /ab*/", "[Parser, NFA]") {
  auto p = Parser("ab*");
  auto nfa = NFA(p.begin(), p.end());
  Plot(nfa, "/tmp/ab_kleene_.dot");
}

TEST_CASE("Parse and build NFA for /a|b*/", "[Parser, NFA]") {
  auto p = Parser("a|b*");
  auto nfa = NFA(p.begin(), p.end());
  Plot(nfa, "/tmp/a_alternate_b_kleene.dot");
}

TEST_CASE("Parse and build NFA for /a|b/", "[Parser, NFA]") {
  auto p = Parser("a|b");
  auto nfa = NFA(p.begin(), p.end());
  Plot(nfa, "/tmp/a_alternate_b.dot");
}

TEST_CASE("Parse and build NFA for /(ac)|b*/", "[Parser, NFA]") {
  auto p = Parser("(ac)|b*");
  auto nfa = NFA(p.begin(), p.end());
  Plot(nfa, "/tmp/open_a_c_close_alternate_b_kleene.dot");
}

TEST_CASE("Epsilon closure for /a|b/", "[NFA]") {
  auto p = Parser("a|b");
  auto nfa = NFA(p.begin(), p.end());
  auto state = nfa.getTransition(NFA::initial, 'a');
  CHECK_NOTHROW(nfa.getEpsilonClosure(state));
  StateSet closure = nfa.getEpsilonClosure(state);
  CHECK(closure.size() == 2);
}
