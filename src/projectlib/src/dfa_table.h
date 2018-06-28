#pragma once

#include "dfa.h"

namespace regex_compiler {
namespace dfa_table {

using State = size_t;
using Input = char;

class DFATable {
  static constexpr size_t NumInputs = 1 << (8 * sizeof(dfa::Input));

  // 2-dimnesional array indexed by state then input.  Element is new state.
  std::vector<std::array<State, NumInputs>> transitions;

  // Do I really want vector of bool?
  std::vector<bool> is_final;

  void add_row();

public:
  static constexpr State initial = 1;

  DFATable(const dfa::DFA &dfa);

  bool accepts(std::string str);
};

}  // namespace dfa_table
}  // namespace regex_compiler
