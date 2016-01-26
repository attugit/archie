#include <archie/fused/if_t.hpp>
#include <catch.hpp>
#include <vector>
#include <string>

namespace {
using namespace archie::fused;
TEST_CASE("static if") {
  auto const size1 = if_(std::true_type{}, [](std::vector<int> v) { return v.size(); },
                         [](std::string s) { return s.size(); })({1, 2, 3});
  REQUIRE(size1 == 3);

  auto const size2 = if_(std::false_type{}, [](std::vector<int> v) { return v.size(); },
                         [](std::string s) { return s.size(); })("Hello");
  REQUIRE(size2 == 5);
}
}
