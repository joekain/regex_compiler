#include "dfa.h"

#include <iostream>
#include <unordered_set>

namespace regex_compiler {
namespace dfa {

State DFA::initial{nfa::NFA::initial};

using NFATransitionSet = std::unordered_set<nfa::Transition>;

// NFATransitionSet

std::unordered_map<Input, State> reachable(const nfa::NFA& nfa, nfa::State from) {
  auto list = nfa.getTransitionsForState(from);
  std::unordered_map<Input, State> map;

  for (nfa::Transition& nfa_transition : list) {
    if (nfa_transition.input != nfa::NFA::epsilon) {
      auto pair = map.emplace(nfa_transition.input, State{});
      pair.first->second.emplace(nfa_transition.new_state);
    } else {
      auto new_map = reachable(nfa, nfa_transition.new_state);
      for (auto&& [input, state] : new_map) {
        auto pair = map.emplace(input, State{});
        pair.first->second.insert(begin(state), end(state));
      }
    }
  }

  return map;
}

void DFA::ensure_node(State s) {
  std::cout << "Ensuring {";
  for (auto sx : s) {
    std::cout << sx << ", ";
  }
  std::cout << "}\n";

  TransitionList l;
  table.emplace(s, l);
}

void DFA::insert(State from, Input input, State to) {
  ensure_node(from);
  ensure_node(to);

  std::cout << "Looking up {";
  for (auto s : from) {
    std::cout << s << ", ";
  }
  std::cout << "}\n";

  std::cout << "inserting " << input << std::endl;

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
      std::cout << "Looking up " << nfa_state << " from nfa\n";
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
