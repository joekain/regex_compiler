#include <sstream>

#include "catch.hpp"

#include "../src/nfa.h"

using namespace std;
using namespace regex_compiler;
using namespace regex_compiler::nfa;

// Determines if an NFA accepts a given input string
class Transitions : public Catch::MatcherBase<NFA> {
  State state_;
  Input input_;

 public:
  Transitions(State state, Input input) : state_(state), input_(input) {
  }

  virtual bool match(NFA const& nfa) const override {
    // Lookup transition table for
    auto transitions = nfa.getTransitionsForState(state_);
    return any_of(std::begin(transitions), std::end(transitions),
                  [this](Transition& t) { return (t.input == this->input_); });
  }

  virtual std::string describe() const override {
    std::ostringstream ss;
    ss << "Transitions from state " << state_ << " on " << input_;
    return ss.str();
  }
};

TEST_CASE("It can build an NFA", "[NFA]") {
  Terms terms{};
  CHECK_NOTHROW(NFA(std::begin(terms), std::end(terms)));
}

TEST_CASE("It can build an NFA from single Literal", "[NFA]") {
  Terms terms{Literal{'a'}};
  auto nfa = NFA(std::begin(terms), std::end(terms));
  CHECK_THAT(nfa, Transitions(NFA::initial, 'a'));
}

TEST_CASE("It can build an NFA for a Kleene closure", "[NFA]") {
  Terms base{Literal{'a'}};
  Terms terms{Kleene{base}};
  auto nfa = NFA(std::begin(terms), std::end(terms));
  CHECK_THAT(nfa, Transitions(NFA::initial, NFA::epsilon));
  CHECK_THAT(nfa, Transitions(1, 'a'));
}

TEST_CASE("It can build an NFA for an Alternative", "[NFA]") {
  Terms alternatives{Literal{'a'}, Literal{'b'}};
  Terms terms{Alternative{alternatives}};
  auto nfa = NFA(std::begin(terms), std::end(terms));
  CHECK_THAT(nfa, Transitions(NFA::initial, 'a'));
  CHECK_THAT(nfa, Transitions(NFA::initial, 'b'));
}

TEST_CASE("It can build an NFA for a sequence", "[NFA]") {
  Terms terms{Literal{'a'}, Literal{'b'}};
  auto nfa = NFA(std::begin(terms), std::end(terms));
  CHECK_THAT(nfa, Transitions(NFA::initial, 'a'));
  auto state = nfa.getTransition(NFA::initial, 'a');
  CHECK_THAT(nfa, Transitions(state, 'b'));
}

TEST_CASE("It can build an NFA for a Group", "[NFA]") {
  Terms sequence{Literal{'a'}, Literal{'b'}};
  Terms terms{Group{sequence}};
  auto nfa = NFA(std::begin(terms), std::end(terms));
  CHECK_THAT(nfa, Transitions(NFA::initial, 'a'));
  auto state = nfa.getTransition(NFA::initial, 'a');
  CHECK_THAT(nfa, Transitions(state, 'b'));
}
