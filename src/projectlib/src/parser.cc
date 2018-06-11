#include "parser.h"

namespace regex_compiler {

static Term parse(std::string::iterator &it, std::string::iterator end);

static Term parse_literal(char c) {
  return Term{Literal{c}};
}

static Terms parse_until(std::string::iterator &it, std::string::iterator end, char end_char) {
  Terms local_terms;
  for (; it != end && *it != end_char; it++) {
    Term t = parse(it, end);
    local_terms.push_back(t);
  }

  return local_terms;
}

static Term parse_group(std::string::iterator &it, std::string::iterator end) {
  Terms group_terms = parse_until(it, end, ')');
  if (*it != ')') throw std::invalid_argument("Unmatched \'(\'");

  return Term{Group{group_terms}};
}

static void parse_kleene_lookahead(std::string::iterator &it, std::string::iterator end, Term &t) {
  if (it != end && *(it + 1) == '*') {
    it += 1;  // skip over *
    t = Term{Kleene{Terms{t}}};
  }
}

static void parse_alternative_lookahead(std::string::iterator &it, std::string::iterator end,
                                        Term &t) {
  if (it != end && *(it + 1) == '|') {
    it += 2;  // skip over |
    Term t2 = parse(it, end);
    t = Term{Alternative{Terms{t, t2}}};
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

Parser::Parser(std::string s) {
  auto it = s.begin();
  terms = parse_until(it, s.end(), 0);
}

}  // namespace regex_compiler
