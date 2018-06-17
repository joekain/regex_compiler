// Builds a DFA from an NFA
#pragma once

#include <list>
#include <map>
#include <set>

#include "nfa.h"

namespace regex_compiler {
namespace dfa {

using Input = nfa::Input;

// A DFA state may be built up of one or more states from an NFA.
using State = std::set<nfa::State>;

struct Transition {
  Input input;
  State new_state;
};

using TransitionList = std::list<Transition>;

using TransitionTable = std::map<State, TransitionList>;

class DFA {
  std::set<State> final_states;
  TransitionTable table;

  void ensure_node(State s);
  void insert(State from, Input input, State to);

 public:
  static State initial;  // = {nfa::NFA::initial};

  DFA(nfa::NFA& nfa);

  TransitionList getTransitionsForState(State state) const {
    return table.at(state);
  }

  TransitionTable::const_iterator begin() const {
    return table.begin();
  }

  TransitionTable::const_iterator end() const {
    return table.end();
  }

  bool accepts(std::string);

  bool is_final(State state) {
    return final_states.find(state) != final_states.end();
  }
};

}  // namespace dfa
}  // namespace regex_compiler
