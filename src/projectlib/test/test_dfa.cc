#include <sstream>

#include "catch.hpp"

#include "../src/dfa.h"
#include "../src/plotter.h"

using namespace std;
using namespace regex_compiler;
using namespace regex_compiler::dfa;

TEST_CASE("It can build a DFA from an NFA", "[DFA]") {
  Terms terms{};
  auto nfa = nfa::NFA(std::begin(terms), std::end(terms));
  CHECK_NOTHROW(DFA(nfa));
}

DFA dfa_from_terms(Terms terms) {
  auto nfa = nfa::NFA(std::begin(terms), std::end(terms));
  return DFA(nfa);
}

TEST_CASE("We can build a DFA using dfa_from_terms", "[DFA]") {
  CHECK_NOTHROW(dfa_from_terms({Literal{'a'}}));
}

std::string to_string(State s) {
  std::ostringstream ss;
  ss << "{";
  for (auto elem : s) {
    ss << elem << " ";
  }
  ss << "}";
  return ss.str();
}

class DFATransitions : public Catch::MatcherBase<DFA> {
  State state_;
  Input input_;

 public:
  DFATransitions(State state, Input input) : state_(state), input_(input) {
  }

  virtual bool match(DFA const& dfa) const override {
    auto transitions = dfa.getTransitionsForState(state_);
    return any_of(std::begin(transitions), std::end(transitions),
                  [this](Transition& t) { return (t.input == this->input_); });
  }

  virtual std::string describe() const override {
    return std::string();
    std::ostringstream ss;
    ss << "Transitions from state " << to_string(state_) << " on " << input_;
    return ss.str();
  }
};

TEST_CASE("It can build a DFA from an NFA of a single literal", "[DFA]") {
  auto dfa = dfa_from_terms({Literal{'a'}});
  CHECK(dfa.getTransitionsForState(DFA::initial).size() == 1);
  CHECK_THAT(dfa, DFATransitions(DFA::initial, 'a'));
  Plotter(dfa, "/tmp/dfa_a.dot");
}

TEST_CASE("It can build a DFA for a Kleene closure", "[DFA]") {
  Terms base{Literal{'a'}};
  Terms terms{Kleene{base}};
  auto dfa = dfa_from_terms(terms);
  CHECK_THAT(dfa, DFATransitions(DFA::initial, 'a'));
  CHECK(dfa.accepts("a"));
  CHECK(dfa.accepts("aa"));
  // CHECK(dfa.accepts(""));
}

TEST_CASE("It can build a DFA for an Alternative", "[DFA]") {
  Terms alternatives{Literal{'a'}, Literal{'b'}};
  Terms terms{Alternative{alternatives}};
  auto dfa = dfa_from_terms(terms);
  CHECK_THAT(dfa, DFATransitions(DFA::initial, 'a'));
  CHECK_THAT(dfa, DFATransitions(DFA::initial, 'b'));
}

TEST_CASE("It can build a DFA for a sequence", "[DFA]") {
  Terms terms{Literal{'a'}, Literal{'b'}};
  auto dfa = dfa_from_terms(terms);
  CHECK_THAT(dfa, DFATransitions(DFA::initial, 'a'));
}

TEST_CASE("It can build a DFA for a Group", "[DFA]") {
  Terms alternatives{Literal{'a'}, Literal{'b'}};
  Terms terms{Group{alternatives}};
  auto dfa = dfa_from_terms(terms);
  CHECK_THAT(dfa, DFATransitions(DFA::initial, 'a'));
}
