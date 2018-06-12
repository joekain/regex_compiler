#pragma once

#include "terms.h"

namespace regex_compiler {

class Parser {
 public:
  Parser(std::string s);

  using iterator = Terms::iterator;

  size_t size() {
    return terms.size();
  }

  iterator begin() noexcept {
    return terms.begin();
  }

  iterator end() noexcept {
    return terms.end();
  }

 private:
  Terms terms;
};
}  // namespace regex_compiler
