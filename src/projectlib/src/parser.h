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
  Term parse_literal(char c) {
    return Term{Literal{c}};
  }

  Terms parse_until(std::string::iterator &it, std::string::iterator end, char end_char) {
    std::vector<Term> local_terms;
    for (; it != end && *it != end_char; it++) {
      std::cout << "Parsing " << *it << std::endl;
      Term t = parse(it, end);
      local_terms.push_back(t);
    }

    if (it == end) {
        std::cout << "Ended on end\n";
    } else {
        std::cout << "Ended on " << *it << std::endl;
    }

    return local_terms;
  }

  Term parse_group(std::string::iterator &it, std::string::iterator end) {
    Terms group_terms = parse_until(it, end, ')');
    std::cout << "Finished group, at " << *it << std::endl;
    if (*it != ')') throw std::invalid_argument("Unmatched \'(\'");

    return Term{Group{group_terms}};
  }

  Term parse(std::string::iterator &it, std::string::iterator end) {
    auto c = *it;
    if (c == '(') {
      it++;
      return parse_group(it, end);
    } else {
      return parse_literal(c);
    }
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
