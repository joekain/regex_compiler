#include "dfa_table.h"

namespace regex_compiler {
namespace dfa_table {

DFATable::DFATable(const dfa::DFA &dfa) {
  // First remap DFA states to DFATable states (simple integers)
  std::map<dfa::State, State> stateMap;
  State state = initial;
  for (const auto &[dfa_state, _] : dfa) {
    stateMap.emplace(dfa_state, state);
    state++;
  }

  // Add a trap state as state 0.  This way anything with unknown input is
  // trapped
  add_row();

  // Now build our table one row at a time
  state = 1;
  for (const auto &[dfa_state, dfa_list] : dfa) {
    add_row();
    for (const auto &transition : dfa_list) {
      transitions[state][transition.input] = stateMap[transition.new_state];
    }
    state++;
  }

  // Build final states
  is_final.reserve(transitions.size());
  std::fill(begin(is_final), end(is_final), false);
  auto dfa_finals = dfa.getFinalStates();
  for (auto &dfa_state : dfa_finals) {
    auto state = stateMap[dfa_state];
    is_final[state] = true;
  }
}

void DFATable::add_row() {
  transitions.emplace_back();
  transitions.back().fill(0);
}

bool DFATable::accepts(std::string str) {
  auto state = initial;
  for (Input input : str) {
    state = transitions[state][input];
  }

  return is_final[state];
}


}  // namespace dfa_table
}  // namespace regex_compiler