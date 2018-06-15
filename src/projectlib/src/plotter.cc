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
    auto list = nfa.getTransitionsForState(state);
    for (auto trans : list) {
      dot << "  " << state << " -> ";
      dot << trans.new_state;
      dot << " [label=";
      if (trans.input) {
        dot << trans.input;
      } else {
        dot << "epsilon";
      }
      dot << "];\n";
    }
  }
}

Plotter::Plotter(const NFA &nfa_, const char *dotfilename) : dot(dotfilename), nfa(nfa_) {
  print_header();
  print_edges();
  print_footer();
}

}  // namespace regex_compiler
