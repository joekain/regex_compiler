#include <string>
#include <vector>
#include <variant>

namespace regex_compiler {

// Literals hold a string of characters
struct Literal {
  char pattern;
};

struct Group;
struct Alternative;
struct Kleene;

// Sort of like variant but with support for recursion
using Term = std::variant<Literal, Group, Alternative, Kleene>;
using Terms = std::vector<Term>;

struct Group {
  Terms pattern;
};

struct Alternative {
  // There will be exactly two
  Terms patterns;
};

struct Kleene {
  // There will be exactly one, vector used in place of pointer
  Terms pattern;
};

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
