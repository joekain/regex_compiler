#include "nfa.h"

namespace regex_compiler {
namespace nfa {

struct Builder {
  TransitionTable table;
  State next_state;
  ClosureMap closures;

  void ensure_node(State s) {
    TransitionList l;
    table.emplace(s, l);
  }

  void insert(State from, Input input, State to) {
    ensure_node(to);
    ensure_node(from);

    auto &list = table.at(from);
    list.emplace_front(input, to);
  }

  State visit_literal(State current_state, const Term &term) {
    auto input = std::get<Literal>(term).pattern;
    insert(current_state, input, ++next_state);

    return next_state;
  }

  State visit_alternative(State current_state, const Term &term) {
    auto terms = std::get<Alternative>(term).patterns;
    State end = ++next_state;
    State last;

    for (int i = 0; i < 2; i++) {
      last = visit_term(current_state, terms[i]);
      insert(last, NFA::epsilon, end);
    }

    return last;
  }

  State visit_kleene(State current_state, const Term &term) {
    Term subTerm = std::get<Kleene>(term).pattern[0];

    // There are 2 extra states for this loop
    State s1 = ++next_state;

    // free transition to s1
    insert(current_state, NFA::epsilon, s1);

    // Build a loop between s1 and s2
    State end = visit_term(s1, subTerm);
    insert(end, NFA::epsilon, s1);

    // Transition from s2 to s3 to end the loop
    State s3 = ++next_state;
    insert(end, NFA::epsilon, s3);

    // Or free transition to s3 for zero occurances
    insert(current_state, NFA::epsilon, s3);

    return s3;
  }

  State visit_group(State current_state, const Term &term) {
    const Terms &terms = std::get<Group>(term).pattern;

    for (auto &term : terms) {
      current_state = visit_term(current_state, term);
    }

    return current_state;
  }

  State visit_term(State current_state, const Term &t) {
    if (std::holds_alternative<Literal>(t)) {
      return visit_literal(current_state, t);
    } else if (std::holds_alternative<Alternative>(t)) {
      return visit_alternative(current_state, t);
    } else if (std::holds_alternative<Kleene>(t)) {
      return visit_kleene(current_state, t);
    } else {
      return visit_group(current_state, t);
    }
  }

  StateSet build_closure(State state) {
    // A state is in its own closure
    StateSet closure{state};

    auto list = table.at(state);
    for (auto transition : list) {
      if (transition.input == NFA::epsilon) {
        auto new_state = transition.new_state;
        if (closure.find(new_state) == closure.end()) {
          // new_state is reachable by epsilon, insert it
          closure.emplace(new_state);

          // the closure of new_state is also reachable, insert it
          auto new_states = build_closure(transition.new_state);
          closure.insert(begin(new_states), end(new_states));
        }
      }
    }
    return closure;
  }

  void build_closures() {
    for (auto state = NFA::initial; state <= next_state; ++state) {
      closures.emplace(state, build_closure(state));
    }
  }

  Builder(Terms::iterator begin, Terms::iterator end) {
    State current_state = NFA::initial;
    next_state = NFA::initial;

    ensure_node(NFA::initial);

    for (auto termIt = begin; termIt != end; termIt++) {
      current_state = visit_term(current_state, *termIt);
    }

    build_closures();
  }
};  // namespace nfa

NFA::NFA(Terms::iterator begin, Terms::iterator end) {
  Builder builder{begin, end};
  table = std::move(builder.table);
  end_state = builder.next_state;
  epsilonClosures = std::move(builder.closures);
}

}  // namespace nfa
}  // namespace regex_compiler
