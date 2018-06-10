#include <string.h>
#include <variant>

namespace regex_compiler {

// Literals hold a string of characters
struct Literal {
  char pattern;
};

using Term = std::variant<Literal>;

class Parser {
 public:
  Parser(std::string s) {
      for (auto c : s) {
        Term t{Literal{c}};
        terms.push_back(t);
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
