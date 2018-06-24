#include <sstream>

#include "plotter.h"

namespace regex_compiler {

struct Plotter {
  std::ofstream dot;

  void print_header();
  void print_edges(const nfa::NFA &nfa);
  void print_edges(const dfa::DFA &dfa);
  template <class State>
  void print_edge(State from, nfa::Input input, State to);
  template <class State>
  void print_final_node(State node);
  void print_final_nodes(const dfa::DFA &dfa);
  void print_footer();

  explicit Plotter(const char *dotfilename) : dot(dotfilename) {
  }
};

void Plotter::print_header() {
  dot << "digraph {\n";
  dot << "  rankdir=LR;\n";
}

void Plotter::print_footer() {
  dot << "}\n";
}

void Plotter::print_edges(const nfa::NFA &nfa) {
  for (auto state = nfa.start(); state <= nfa.final(); ++state) {
    for (auto trans : nfa.getTransitionsForState(state)) {
      print_edge(state, trans.input, trans.new_state);
    }
  }
}

void Plotter::print_edges(const dfa::DFA &dfa) {
  for (auto &&[state, _] : dfa) {
    for (auto trans : dfa.getTransitionsForState(state)) {
      print_edge(state, trans.input, trans.new_state);
    }
  }
}

std::string to_string(nfa::State state) {
  return std::to_string(state);
}

std::string to_string(dfa::State s) {
  std::ostringstream ss;
  ss << "{";
  for (auto elem : s) {
    ss << elem << " ";
  }
  ss << "}";
  return ss.str();
}

template <class State>
void Plotter::print_edge(State from, nfa::Input input, State to) {
  dot << "  \"" << to_string(from) << "\" -> ";
  dot << "\"" << to_string(to) << "\"";
  dot << " [label=";
  if (input) {
    dot << input;
  } else {
    dot << "epsilon";
  }
  dot << "];\n";
}

template <class State>
void Plotter::print_final_node(State node) {
  dot << "  \"" << to_string(node) << "\" [shape=doublecircle];\n";
}

void Plotter::print_final_nodes(const dfa::DFA &dfa) {
  for (const auto &state : dfa.getFinalStates()) {
    print_final_node(state);
  }
}

void Plot(const nfa::NFA &nfa, const char *dotfilename) {
  Plotter p(dotfilename);
  p.print_header();
  p.print_edges(nfa);
  p.print_final_node(nfa.final());
  p.print_footer();
}

void Plot(const dfa::DFA &dfa, const char *dotfilename) {
  Plotter p(dotfilename);
  p.print_header();
  p.print_edges(dfa);
  p.print_final_nodes(dfa);
  p.print_footer();
}

}  // namespace regex_compiler
