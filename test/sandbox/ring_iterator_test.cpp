#include <archie/container/ring_iterator.hpp>
#include <catch.hpp>
namespace {
using namespace archie;
TEST_CASE("ring_iterator", "[ring]") {
  using sut = ring_iterator<std::vector<int>::iterator>;
  SECTION("default ctor") {
    std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6};
    sut it0;
    sut it1(std::begin(vec), std::end(vec));
    sut it2(std::begin(vec), std::end(vec), 2);
    sut it3(std::begin(vec), std::end(vec), 2 + vec.size());
    REQUIRE_FALSE(it0 == it1);
    REQUIRE(it0 != it1);
    REQUIRE_FALSE(it1 == it2);
    REQUIRE(it1 != it2);
    REQUIRE_FALSE(it2 == it3);
    REQUIRE(it2 != it3);
    REQUIRE((&(*it2)) == (&(*it3)));
    REQUIRE(norm(it2) == norm(it3));
  }
  SECTION("increment") {
    using buff_t = std::vector<int>;
    buff_t buff = {0, 1, 2, 3, 4, 5, 6};
    ring_iterator<buff_t::const_iterator> it(buff.begin() + 1, buff.begin() + 4);
    REQUIRE(*it++ == 1);
    REQUIRE(*it++ == 2);
    REQUIRE(*it++ == 3);
    REQUIRE(*it++ == 1);
  }
}
}
