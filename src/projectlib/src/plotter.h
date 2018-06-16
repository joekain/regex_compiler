// Plots an NFA

#pragma once

#include <fstream>

#include "dfa.h"
#include "nfa.h"

namespace regex_compiler {

class Plotter {
  std::ofstream dot;

  void print_header();
  void print_edges(const nfa::NFA &nfa);
  void print_edges(const dfa::DFA &dfa);
  template <class State>
  void print_edge(State from, nfa::Input input, State to);
  void print_footer();

 public:
  Plotter(const nfa::NFA &nfa, const char *dotfilename);
  Plotter(const dfa::DFA &dfa, const char *dotfilename);
};

}  // namespace regex_compiler
