#include <iostream>
#include <sstream>
#include <unordered_set>

#include "dfa.h"

namespace regex_compiler {
namespace dfa {

using InputSet = std::unordered_set<Input>;

struct Builder {
  State initial;
  nfa::NFA& nfa_;
  std::list<State> queue_;
  TransitionTable table;

  void push(const State& s) {
    queue_.push_back(s);
  }

  State pop() {
    State val = queue_.front();
    queue_.pop_front();
    return val;
  }

  bool empty() {
    return queue_.empty();
  }

  InputSet inputsForState(State dfa_state) {
    InputSet set;
    for (auto nfa_state : dfa_state) {
      for (auto transition : nfa_.getTransitionsForState(nfa_state)) {
        if (transition.input != nfa::NFA::epsilon) {
          set.emplace(transition.input);
        }
      }
    }
    return set;
  }

  State buildTransition(State from, Input input) {
    State to;

    for (auto nfa_state : from) {
      auto nfa_transitions = nfa_.getTransitionsForState(nfa_state);
      for (auto nfa_transition : nfa_transitions) {
        if (input != nfa_transition.input) continue;
        auto closure = nfa_.getEpsilonClosure(nfa_transition.new_state);
        to.insert(begin(closure), end(closure));
      }
    }

    return to;
  }

  bool haveState(const State& s) {
    return table.find(s) != table.end();
  }

  void ensure_node(State s) {
    TransitionList l;
    table.emplace(s, l);
  }

  void insert(const State& from, Input input, const State& to) {
    ensure_node(from);
    ensure_node(to);

    auto& list = table.at(from);
    Transition t{input, to};
    list.push_front(t);
  }

  Builder(nfa::NFA& nfa) : nfa_(nfa) {
    initial = nfa_.getEpsilonClosure(nfa::NFA::initial);
    push(initial);
    while (!empty()) {
      State from = pop();

      for (auto input : inputsForState(from)) {
        State to = buildTransition(from, input);
        if (!haveState(to)) {
          push(to);
        }
        insert(from, input, to);
      }
    }
  }
};

DFA::DFA(nfa::NFA& nfa) {
  auto builder = Builder(nfa);
  table = std::move(builder.table);
  initial = std::move(builder.initial);

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
