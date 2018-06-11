#include <string>
#include <variant>
#include <vector>

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
}  // namespace regex_compiler
