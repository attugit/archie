#include <archie/fused/for_each.h>
#include <string>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

struct func {
  int cnt = 0;
  template <typename Tp>
  void operator()(Tp&&) noexcept {
    ++cnt;
  }
};

TEST_CASE("canCallForEachWithFunctionObject") {
  func f;
  REQUIRE(0 == f.cnt);
  fused::for_each(f);
  REQUIRE(0 == f.cnt);
  auto& ret = fused::for_each(f, '7', 'c', std::string{});
  REQUIRE(3 == f.cnt);
  REQUIRE(3 == ret.cnt);
  REQUIRE(&f == &ret);
}

TEST_CASE("canCallForEachWithLambda") {
  int idx = 0;
  auto f = [&idx](auto&&) { ++idx; };
  REQUIRE(0 == idx);
  fused::for_each(f, 7, 'c', 2.0);
  REQUIRE(3 == idx);
}
}
