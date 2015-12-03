#include <archie/opaque.hpp>
#include <catch.hpp>
#include <functional>

namespace {
using namespace archie;
struct ATag {};
struct BTag {};
struct CTag {};

using BOpaque = opaque<BTag, double, feature::equivalent<tag<ATag>>, feature::equivalent<self>>;

using AOpaque = opaque<ATag,
                       double,
                       feature::extractable,
                       feature::equivalent<feature::self>,
                       feature::equivalent<double>>;

using COpaque = opaque<CTag,
                       double,
                       feature::extractable,
                       feature::assignable<double>,
                       feature::ordered<feature::self>,
                       feature::ordered<double>,
                       feature::ordered<tag<ATag>>>;

TEST_CASE("constructors", "[opaque]") {
  SECTION("default ctor") {
    AOpaque aopq;
    REQUIRE(sizeof(aopq) == sizeof(double));
  }
  SECTION("param ctor") {
    AOpaque aopq(4.0);
    REQUIRE(sizeof(aopq) == sizeof(double));
  }
  SECTION("copy ctor") {
    AOpaque const aopq(5.0);
    AOpaque other = aopq;
    REQUIRE(sizeof(other) == sizeof(double));
  }
  SECTION("move ctor") {
    AOpaque aopq(5.0);
    AOpaque other = std::move(aopq);
    REQUIRE(sizeof(other) == sizeof(double));
  }
}

TEST_CASE("extractable", "[opaque]") {
  SECTION("extract const opaque") {
    AOpaque const aopq(4.0);
    REQUIRE(extract(aopq) == 4.0);
  }
  SECTION("extract non const opaque") {
    AOpaque aopq(4.0);
    extract(aopq) = 3.0;
    REQUIRE(extract(aopq) == 3.0);
  }
  SECTION("extract non-opaque type") {
    double const x = 10.0;
    auto const& y = extract(x);
    REQUIRE(&x == &y);
    double z = 3.0;
    auto& v = extract(z);
    v = 5.0;
    REQUIRE(&z == &v);
  }
  SECTION("extract non-extractable opaque") {
    BOpaque const x(1.0);
    auto const& y = extract(x);
    REQUIRE(&x == &y);
    BOpaque z(1.0);
    BOpaque& v = extract(z);
    REQUIRE(&v == &z);
  }
}

TEST_CASE("operators", "[opaque]") {
  AOpaque const aopq(3.0);
  BOpaque const bopq(4.0);
  SECTION("eq comparable") {
    REQUIRE(aopq == 3.0);
    REQUIRE(aopq == aopq);
    REQUIRE(aopq != 4.0);
    REQUIRE(bopq != aopq);
  }
  SECTION("less comparable") {
    AOpaque const a{5.0};
    COpaque const x{2.0};
    COpaque const y{3.0};

    REQUIRE(x < y);
    REQUIRE(x <= y);
    REQUIRE(x <= x);
    REQUIRE(y > x);
    REQUIRE(y >= x);
    REQUIRE(y >= y);

    REQUIRE_FALSE(y < x);
    REQUIRE_FALSE(x < x);
    REQUIRE_FALSE(y <= x);
    REQUIRE_FALSE(x > y);
    REQUIRE_FALSE(x > x);
    REQUIRE_FALSE(x >= y);

    REQUIRE(x < 3.0);
    REQUIRE(x <= 3.0);
    REQUIRE(x <= 2.0);
    REQUIRE(x > 1.0);
    REQUIRE(x >= 1.0);
    REQUIRE(x >= 2.0);

    REQUIRE(1.0 < x);
    REQUIRE(1.0 <= x);
    REQUIRE(2.0 <= x);
    REQUIRE(3.0 > x);
    REQUIRE(3.0 >= x);
    REQUIRE(2.0 >= x);

    REQUIRE_FALSE(x < 1.0);
    REQUIRE_FALSE(x < 2.0);
    REQUIRE_FALSE(x <= 1.0);
    REQUIRE_FALSE(x > 3.0);
    REQUIRE_FALSE(x > 2.0);
    REQUIRE_FALSE(x >= 3.0);

    REQUIRE_FALSE(3.0 < x);
    REQUIRE_FALSE(2.0 < x);
    REQUIRE_FALSE(3.0 <= x);
    REQUIRE_FALSE(1.0 > x);
    REQUIRE_FALSE(2.0 > x);
    REQUIRE_FALSE(1.0 >= x);

    REQUIRE(x < a);
    REQUIRE(x <= a);
    REQUIRE_FALSE(x > a);
    REQUIRE_FALSE(x >= a);
    REQUIRE(a > x);
    REQUIRE(a >= x);
    REQUIRE_FALSE(a < x);
    REQUIRE_FALSE(a <= x);
  }
  SECTION("assignable") {
    AOpaque other(3.0);
    REQUIRE(other == 3.0);
    extract(other) = 4.0;
    REQUIRE(other == 4.0);
    other = AOpaque{5.0};
    REQUIRE(other == 5.0);
    COpaque copq(2.0);
    copq = 6.0;
    REQUIRE(extract(copq) == 6.0);
  }
}
}
