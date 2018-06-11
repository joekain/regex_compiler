#include <string.h>
#include <variant>
#include <iostream>

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
    std::vector<Term> pattern;
};

struct Alternative {
  // There will be exactly two
  std::vector<Term> patterns;
};

struct Kleene {
    // There will be exactly one, vector used in place of pointer
    std::vector<Term> pattern;
};

class Parser {
  Term parse_literal(char c) {
    return Term{Literal{c}};
  }

  Terms parse_until(std::string::iterator &it, std::string::iterator end,
                    char end_char) {
    std::vector<Term> local_terms;
    for (; it != end && *it != end_char; it++) {
      Term t = parse(it, end);
      local_terms.push_back(t);
    }

    return local_terms;
  }

  Term parse_group(std::string::iterator &it, std::string::iterator end) {
    Terms group_terms = parse_until(it, end, ')');
    if (*it != ')') throw std::invalid_argument("Unmatched \'(\'");

    return Term{Group{group_terms}};
  }

  void parse_kleene_lookahead(std::string::iterator &it,
                              std::string::iterator end, Term &t) {
    if (it != end && *(it + 1) == '*') {
      it += 1;  // skip over *
      t = Term{Kleene{std::vector<Term>{t}}};
    }
  }

  void parse_alternative_lookahead(std::string::iterator &it,
                                   std::string::iterator end, Term &t) {
    if (it != end && *(it + 1) == '|') {
      it += 2;  // skip over |
      Term t2 = parse(it, end);
      t = Term{Alternative{std::vector<Term>{t, t2}}};
    }
  }

  Term parse(std::string::iterator &it, std::string::iterator end) {
    auto c = *it;
    Term t;
    if (c == '(') {
      it++;
      t = parse_group(it, end);
    } else {
      t = parse_literal(c);
    }

    parse_kleene_lookahead(it, end, t);
    parse_alternative_lookahead(it, end, t);

    return t;
  }

 public:
  Parser(std::string s) {
    auto it = s.begin();
    terms = parse_until(it, s.end(), 0);
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
