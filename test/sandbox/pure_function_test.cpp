#include <archie/pure_function.hpp>
#include <catch.hpp>
#include <memory>

namespace {
using namespace archie;

using arg_t = std::unique_ptr<int>;

int foo(arg_t) { return 2; }

struct bar {
  int operator()(arg_t) const { return 7; };
} boo;

using pf_t = pure_function<int(arg_t)>;

TEST_CASE("pure_function construction", "[pure_function]") {
  SECTION("can construct from free function") {
    pf_t pf{foo};
    REQUIRE(pf);
    REQUIRE(pf(std::make_unique<int>(0)) == foo(std::make_unique<int>(0)));
  }
  SECTION("can assign captureless lambda") {
    auto const l = [](arg_t) { return 3; };
    pf_t pf{l};
    REQUIRE(pf);
    REQUIRE(pf(std::make_unique<int>(0)) == l(std::make_unique<int>(0)));
  }
  SECTION("can assing custom function object") {
    pf_t pf{boo};
    REQUIRE(pf);
    REQUIRE(pf(std::make_unique<int>(0)) == boo(std::make_unique<int>(0)));
  }
}

TEST_CASE("pure_function assigment", "[pure_function]") {
  pf_t pf;
  SECTION("default constructed pure_function is null") { REQUIRE(!pf); }
  SECTION("can assign free function") {
    pf = foo;
    REQUIRE(pf);
    REQUIRE(pf(std::make_unique<int>(0)) == foo(std::make_unique<int>(0)));
  }
  SECTION("can assign captureless lambda") {
    auto const l = [](arg_t) { return 3; };
    pf = l;
    REQUIRE(pf);
    REQUIRE(pf(std::make_unique<int>(0)) == l(std::make_unique<int>(0)));
  }
  SECTION("can assing custom function object") {
    pf = boo;
    REQUIRE(pf);
    REQUIRE(pf(std::make_unique<int>(0)) == boo(std::make_unique<int>(0)));
  }
  SECTION("can assign nullptr") {
    pf = nullptr;
    REQUIRE(!pf);
  }
}

TEST_CASE("pure_function conversion to function pointer", "[pure_function]") {
  pf_t pf;
  pf_t::pointer ptr = pf;
  REQUIRE(!ptr);
  pf = boo;
  ptr = pf;
  REQUIRE(ptr);
  REQUIRE(ptr(std::make_unique<int>(0)) == boo(std::make_unique<int>(0)));
}
}
