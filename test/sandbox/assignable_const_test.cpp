#include <archie/assignable_const.hpp>
#include <catch.hpp>

namespace {
using namespace archie;

struct block_t {
  block_t(unsigned n) : number(n) {}
  assignable_const<unsigned> number;
};

TEST_CASE("assignable_const", "[const]") {
  block_t b1(3);
  block_t b2(4);

  REQUIRE(b1.number == 3);
  b1 = b2;
  REQUIRE(b1.number == 4);

  REQUIRE(std::is_copy_constructible<block_t>::value);
  REQUIRE(std::is_copy_assignable<block_t>::value);
}
}
