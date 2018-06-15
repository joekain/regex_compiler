// Plots an NFA

#pragma once

#include <fstream>

#include "nfa.h"

namespace regex_compiler {

class Plotter {
  std::ofstream dot;
  const NFA &nfa;

  void print_header();
  void print_edges();
  void print_footer();

 public:
  Plotter(const NFA &nfa, const char *dotfilename);
};

}  // namespace regex_compiler
