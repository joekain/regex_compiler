#include "catch.hpp"

#include <string>
#include "../src/parser.h"

using namespace std;
using namespace regex_compiler;

TEST_CASE("The empty string can be parsed", "[Parser]") {
    auto p = Parser("");
}
