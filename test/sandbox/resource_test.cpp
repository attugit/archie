#include <archie/resource.hpp>
#include <archie/alias.hpp>
#include <catch.hpp>
#include <functional>
namespace {
using namespace archie;

TEST_CASE("Can use resource", "[resource]") {
  auto const del = [](auto& a) { ++unwrap(a); };
  SECTION("Can create resource") {
    int i = 0;
    {
      auto res = make_resource(alias(i), del);
      REQUIRE(i == 0);
    }
    REQUIRE(i == 1);
  }

  SECTION("Can release resource") {
    int i = 0;
    {
      auto res = make_resource(alias(i), del);
      REQUIRE(i == 0);
      res.release();
    }
    REQUIRE(i == 0);
  }

  SECTION("Can assign null to resource") {
    int i = 0;
    {
      auto res = make_resource(alias(i), del);
      REQUIRE(i == 0);
      res = null_resource;
    }
    REQUIRE(i == 0);
  }

  SECTION("Can engage resource") {
    int i = 0;
    {
      auto res = resource<int, std::function<void(int&)>>(in_place, 4);
      res.engage([&i](auto& x) { i += x; });
      REQUIRE(i == 0);
    }
    REQUIRE(i == 4);
  }
}
}
