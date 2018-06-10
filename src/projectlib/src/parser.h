#include <string.h>
#include <variant>
#include <iostream>

namespace regex_compiler {

// Literals hold a string of characters
struct Literal {
  char pattern;
};

struct Group;

// Sort of like variant but with support for recursion
using Term = std::variant<Literal, Group>;
using Terms = std::vector<Term>;

struct Group {
    std::vector<Term> pattern;
};

class Parser {

public:
  Parser(std::string s) {
    for (auto it = s.begin(); it != s.end(); it++) {
      auto c = *it;
      if (c == '(') {
        std::cout << "Found (\n";
        std::vector<Term> group_terms;
        for (; it != s.end() && *it != ')'; it++) {
          auto c = *it;
          Term t{Literal{c}};
          group_terms.push_back(t);
        }
        Term t{Group{group_terms}};
        terms.push_back(t);
      } else {
        std::cout << "Found " << c << std::endl;
        Term t{Literal{c}};
        terms.push_back(t);
      }
    }
  }

  using iterator = std::vector<Term>::iterator;

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
  std::vector<Term> terms;
};
}  // namespace regex_compiler
