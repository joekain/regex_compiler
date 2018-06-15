#include "plotter.h"

namespace regex_compiler {

void Plotter::print_header() {
  dot << "digraph {\n";
  dot << "  rankdir=LR;\n";
}

void Plotter::print_footer() {
  dot << "}\n";
}

void Plotter::print_edges() {
  for (auto state = nfa.start(); state <= nfa.final(); ++state) {
    for (auto trans : nfa.getTransitionsForState(state)) {
      print_edge(state, trans.input, trans.new_state);
    }
  }
}

void Plotter::print_edge(State from, Input input, State to) {
  dot << "  " << from << " -> ";
  dot << to;
  dot << " [label=";
  if (input) {
    dot << input;
  } else {
    dot << "epsilon";
  }
  dot << "];\n";
}

Plotter::Plotter(const NFA &nfa_, const char *dotfilename) : dot(dotfilename), nfa(nfa_) {
  print_header();
  print_edges();
  print_footer();
}

}  // namespace regex_compiler
