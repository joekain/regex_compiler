// Plots an NFA

#pragma once

#include <fstream>

#include "nfa.h"

namespace regex_compiler {

class Plotter {
  std::ofstream dot;
  const nfa::NFA &nfa;

  void print_header();
  void print_edges();
  void print_edge(nfa::State from, nfa::Input input, nfa::State to);
  void print_footer();

 public:
  Plotter(const nfa::NFA &nfa, const char *dotfilename);
};

}  // namespace regex_compiler
