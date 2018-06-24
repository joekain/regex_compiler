#include <iostream>
#include <sstream>
#include <unordered_set>

#include "dfa.h"

namespace regex_compiler {
namespace dfa {

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

static State reachable_by(const nfa::NFA& nfa, nfa::State from, nfa::Input input) {
  auto list = nfa.getTransitionsForState(from);
  State states;

  for (nfa::Transition& nfa_transition : list) {
    if (nfa_transition.input == input) {
      states.emplace(nfa_transition.new_state);
    }
    if (nfa_transition.input == nfa::NFA::epsilon) {
      auto new_states = reachable_by(nfa, nfa_transition.new_state, input);
      states.insert(begin(new_states), end(new_states));
    }
  }

  return states;
}

static void extend(const nfa::NFA& nfa, State& from) {
  for (auto nfa_state : from) {
    auto epsilon_states = reachable_by(nfa, nfa_state, nfa::NFA::epsilon);
    from.insert(begin(epsilon_states), end(epsilon_states));
  }
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

static std::string to_string(State s) {
  std::ostringstream ss;
  ss << "{";
  for (auto elem : s) {
    ss << elem << " ";
  }
  ss << "}";
  return ss.str();
}

DFA::DFA(nfa::NFA& nfa) : initial{nfa::NFA::initial} {
  std::list<State> queue;

  // Build epsilon closures for all NFA states

  queue.push_back(initial);
  while (!queue.empty()) {
    State from = queue.front();
    queue.pop_front();
    extend(nfa, from);
    ensure_node(from);

    // need to merge all the reachables before generating the states and transitions
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

  // Build final states - a state is final if it contains the final NFA state
  for (auto&& [state, _] : table) {
    if (state.find(nfa.final()) != state.end()) {
      final_states.emplace(state);
    }
  }
}

bool DFA::accepts(std::string pattern) {
  // Traverse the DFA and see if end up in a final state
  State current_state = initial;

  for (Input input : pattern) {
    auto list = getTransitionsForState(current_state);
    bool found = false;
    for (auto& transition : list) {
      if (transition.input == input) {
        found = true;
        current_state = transition.new_state;
        break;
      }
    }

    if (!found) return false;
  }

  return is_final(current_state);
}

}  // namespace dfa
}  // namespace regex_compiler
