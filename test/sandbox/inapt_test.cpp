#if 0 // usage example
#include <archie/inapt.hpp>
#include <iostream>
struct error {
  enum codes : int { a = -1, b = -2, c = -3 };
  static void handle(int x) { handle(static_cast<codes>(x)); }
  static void handle(codes c) {
    switch (c) {
      case codes::a:
        std::cout << "Error A!\n";
        break;
      case codes::b:
        std::cout << "Error B!\n";
        break;
      case codes::c:
        std::cout << "Error C!\n";
        break;
      default:
        std::cout << "Unknown error!\n";
    };
  }
};

using status = archie::reserved_t<int, error::codes::a, error::codes::b, error::codes::c>;

status foo() {
  static int x = error::codes::c;
  return status(x++);
}

int main() {
  status s;
  while (!s) {
    error::handle(s.get());
    s = foo();
  }
  std::cout << "Test passed! status: " << s.get() << "\n";
  return 0;
}
#endif

#include <archie/inapt.hpp>
#include <catch.hpp>
namespace {
using namespace archie;
struct error {
  enum codes : int { a = -1, b = -2, c = -3 };
};

using status = reserved_t<int, error::codes::a, error::codes::b, error::codes::c>;

TEST_CASE("Can create inapt", "[inapt]") {
  enum { non_null = 7 };
  status s;
  null_inapt_t null;

  SECTION("Default value is null") { REQUIRE(s == null); }

  SECTION("Can assign other null value") {
    REQUIRE(s != error::codes::b);
    s = error::codes::b;
    REQUIRE(s == error::codes::b);
    REQUIRE(s == null);
  }

  SECTION("Can assign nonnull value") {
    REQUIRE(s != non_null);
    s = non_null;
    REQUIRE(s == non_null);
    REQUIRE(s != null);
  }

  SECTION("Can construct nonull value") {
    status nn{non_null};
    REQUIRE(nn == non_null);
    REQUIRE(nn != null);
  }

  SECTION("Can assing null_inapt_t") {
    s = non_null;
    REQUIRE(s != null);
    s = null;
    REQUIRE(s == null);
  }
}
}
