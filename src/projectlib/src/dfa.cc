#include <iostream>
#include <sstream>
#include <unordered_set>

#include "dfa.h"

namespace regex_compiler {
namespace dfa {

DFA::DFA(nfa::NFA& nfa) : initial{nfa::NFA::initial} {
  // Build epsilon closures for all NFA states
  // Add {nfa::initial} as dfa::initial
  //   For each input, I
  //     Consider IE*, this really means
  //       For each NFA transition for I
  //         Look a NFA(state, I) and get that new states E closure

  std::list<State> queue;
  queue.push_back(initial);
  while (!queue.empty()) {
    State from = queue.front();
    queue.pop_front();
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
