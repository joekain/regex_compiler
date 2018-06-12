// Builds an NFA from a Regex tree stored as Terms
// An NFA is a non-deterministic Finite Automaton.
//
// - Transitions  : A map that describes transitions between states
// - final states : A list of states considered to be final
#pragma once

#include <list>
#include <unordered_map>

#include "terms.h"

namespace regex_compiler {

// The NFA consists of states and transitions, based on inputs, from
// those states to other stats.
//
// We store a table, a map, indexed by state.  The entry for the state
// is a list of all possible transitions from that state.
//
// A state is just a number.  The initial state is 0.
using State = size_t;

// An input is a character
using Input = char;

// A transition is a char and the new state it takes us to
struct Transition {
  Input input;
  State new_state;

  Transition(Input i, State s) : input(i), new_state(s) {
  }
};

// A table entry is a list of transitions
using TransitionList = std::list<Transition>;

// A transition table is a map of states to list of possible transitions
using TransitionTable = std::unordered_map<State, TransitionList>;

class NFA {
 public:
  NFA(Terms::iterator begin, Terms::iterator end){};
};

}  // namespace regex_compiler
