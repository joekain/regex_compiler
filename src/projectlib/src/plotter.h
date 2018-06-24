// Plots an NFA

#pragma once

#include <fstream>

#include "dfa.h"
#include "nfa.h"

namespace regex_compiler {

void Plot(const nfa::NFA &nfa, const char *dotfilename);
void Plot(const dfa::DFA &nfa, const char *dotfilename);

}  // namespace regex_compiler
