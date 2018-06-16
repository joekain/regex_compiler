#include "dfa.h"

#include <iostream>
#include <unordered_set>

namespace regex_compiler {
namespace dfa {

State DFA::initial{nfa::NFA::initial};

using NFATransitionSet = std::unordered_set<nfa::Transition>;

using StatesByInput = std::unordered_map<Input, State>;

static StatesByInput reachable(const nfa::NFA& nfa, nfa::State from) {
  auto list = nfa.getTransitionsForState(from);
  StatesByInput map;

  for (nfa::Transition& nfa_transition : list) {
    if (nfa_transition.input != nfa::NFA::epsilon) {
      auto&& [it, _] = map.emplace(nfa_transition.input, State{});
      it->second.emplace(nfa_transition.new_state);
    } else {
      auto new_map = reachable(nfa, nfa_transition.new_state);
      for (auto&& [input, state] : new_map) {
        auto&& [it, _] = map.emplace(input, State{});
        it->second.insert(begin(state), end(state));
      }
    }
  }

  return map;
}

void DFA::ensure_node(State s) {
  TransitionList l;
  table.emplace(s, l);
}

void DFA::insert(State from, Input input, State to) {
  ensure_node(from);
  ensure_node(to);

  auto& list = table.at(from);
  Transition t{input, to};
  list.push_front(t);
}

DFA::DFA(nfa::NFA& nfa) {
  std::list<State> queue;

  ensure_node(initial);
  queue.push_back(initial);
  while (!queue.empty()) {
    State from = queue.front();
    queue.pop_front();

    for (auto nfa_state : from) {
      auto transitions_by_input = reachable(nfa, nfa_state);

      // The sets of transitions are dfa:State objects
      for (auto&& [input, to] : transitions_by_input) {
        if (table.find(to) == table.end()) {
          queue.push_back(to);
        }
        insert(from, input, to);
      }
    }
  }
}

}  // namespace dfa
}  // namespace regex_compiler
