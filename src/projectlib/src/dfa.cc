#include "dfa.h"

#include <iostream>
#include <unordered_set>

namespace regex_compiler {
namespace dfa {

State DFA::initial{nfa::NFA::initial};

using NFATransitionSet = std::unordered_set<nfa::Transition>;

using StatesByInput = std::unordered_map<Input, State>;

static StatesByInput reachable(const nfa::NFA& nfa, nfa::State from);

static void insert_new_transition(StatesByInput& map, nfa::Transition transition) {
  auto&& [it, _] = map.emplace(transition.input, State{});
  it->second.emplace(transition.new_state);
}

static void insert_recursive_transitions(StatesByInput& map, const nfa::NFA& nfa,
                                         nfa::State new_state) {
  auto new_map = reachable(nfa, new_state);
  for (auto&& [input, state] : new_map) {
    auto&& [it, _] = map.emplace(input, State{});
    it->second.insert(begin(state), end(state));
  }
}

static StatesByInput reachable(const nfa::NFA& nfa, nfa::State from) {
  auto list = nfa.getTransitionsForState(from);
  StatesByInput map;

  for (nfa::Transition& nfa_transition : list) {
    if (nfa_transition.input != nfa::NFA::epsilon) {
      insert_new_transition(map, nfa_transition);
    } else {
      insert_recursive_transitions(map, nfa, nfa_transition.new_state);
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
      // The reachable sets of transitions are dfa:State objects
      for (auto&& [input, to] : reachable(nfa, nfa_state)) {
        // Queue any new states for further traversal
        if (table.find(to) == table.end()) {
          queue.push_back(to);
        }

        // Insert the transition into our table
        insert(from, input, to);
      }
    }
  }
}

}  // namespace dfa
}  // namespace regex_compiler
