#include <archie/fused/zip.hpp>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

TEST_CASE("canZipTwoTuples", "[fused::zip]")
{
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip(a, b);

  REQUIRE(3u == fused::tuple_size(x));
  REQUIRE(fused::make_tuple(1, '4') == fused::get<0>(x));
  REQUIRE(fused::make_tuple(2u, 5.0) == fused::get<1>(x));
  REQUIRE(fused::make_tuple('3', 6u) == fused::get<2>(x));
}

TEST_CASE("canZipManyTuples", "[fused::zip]")
{
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple(4u, '5', 6);
  auto c = fused::make_tuple('7', 8, 9u);
  auto x = fused::zip(a, b, c);

  REQUIRE(3u == fused::tuple_size(x));
  REQUIRE(fused::make_tuple(1, 4u, '7') == fused::get<0>(x));
  REQUIRE(fused::make_tuple(2u, '5', 8) == fused::get<1>(x));
  REQUIRE(fused::make_tuple('3', 6, 9u) == fused::get<2>(x));
}

TEST_CASE("canZipViewTwoTuples", "[fused::zip]")
{
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip_view(a, b);

  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  auto const& x0 = fused::get<0>(x);
  auto const& x1 = fused::get<1>(x);
  auto const& x2 = fused::get<2>(x);

  static_assert(std::is_same<decltype(x0), fused::tuple<int&, char&> const&>::value, "");
  static_assert(std::is_same<decltype(x1), fused::tuple<unsigned&, double&> const&>::value, "");
  static_assert(std::is_same<decltype(x2), fused::tuple<char&, unsigned&> const&>::value, "");

  REQUIRE(&fused::get<0>(a) == &fused::get<0>(x0));
  REQUIRE(&fused::get<1>(a) == &fused::get<0>(x1));
  REQUIRE(&fused::get<2>(a) == &fused::get<0>(x2));

  REQUIRE(&fused::get<0>(b) == &fused::get<1>(x0));
  REQUIRE(&fused::get<1>(b) == &fused::get<1>(x1));
  REQUIRE(&fused::get<2>(b) == &fused::get<1>(x2));

  fused::get<0>(x1) = 7u;
  REQUIRE(7u == fused::get<1>(a));
  fused::get<1>(x2) = 8u;
  REQUIRE(8u == fused::get<2>(b));
}

TEST_CASE("canZipViewManyTuples", "[fused::zip]")
{
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple(4u, '5', 6.0);
  auto c = fused::make_tuple('7', 8.0, 9u);
  auto x = fused::zip_view(a, b, c);

  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  auto const& x0 = fused::get<0>(x);
  auto const& x1 = fused::get<1>(x);
  auto const& x2 = fused::get<2>(x);

  static_assert(std::is_same<decltype(x0), fused::tuple<int&, unsigned&, char&> const&>::value, "");
  static_assert(std::is_same<decltype(x1), fused::tuple<unsigned&, char&, double&> const&>::value,
                "");
  static_assert(std::is_same<decltype(x2), fused::tuple<char&, double&, unsigned&> const&>::value,
                "");

  REQUIRE(&fused::get<0>(a) == &fused::get<0>(x0));
  REQUIRE(&fused::get<1>(a) == &fused::get<0>(x1));
  REQUIRE(&fused::get<2>(a) == &fused::get<0>(x2));

  REQUIRE(&fused::get<0>(b) == &fused::get<1>(x0));
  REQUIRE(&fused::get<1>(b) == &fused::get<1>(x1));
  REQUIRE(&fused::get<2>(b) == &fused::get<1>(x2));

  REQUIRE(&fused::get<0>(c) == &fused::get<2>(x0));
  REQUIRE(&fused::get<1>(c) == &fused::get<2>(x1));
  REQUIRE(&fused::get<2>(c) == &fused::get<2>(x2));

  fused::get<0>(x1) = 10u;
  REQUIRE(10u == fused::get<1>(a));
  fused::get<1>(x2) = 11.0;
  REQUIRE(11.0 == fused::get<2>(b));
}

TEST_CASE("canZipViewTwoConstTuples", "[fused::zip]")
{
  auto const a = fused::make_tuple(1, 2u, '3');
  auto const b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip_view(a, b);

  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  auto x0 = fused::get<0>(x);
  auto x1 = fused::get<1>(x);
  auto x2 = fused::get<2>(x);

  REQUIRE(&fused::get<0>(a) == &fused::get<0>(x0));
  REQUIRE(&fused::get<1>(a) == &fused::get<0>(x1));
  REQUIRE(&fused::get<2>(a) == &fused::get<0>(x2));
  REQUIRE(&fused::get<0>(b) == &fused::get<1>(x0));
  REQUIRE(&fused::get<1>(b) == &fused::get<1>(x1));
  REQUIRE(&fused::get<2>(b) == &fused::get<1>(x2));
}

TEST_CASE("canZipViewTwoTuplesWithConsts", "[fused::zip]")
{
  auto const c = 2u;
  auto const d = 5.0;
  auto a = fused::tuple<int, const unsigned, char>{1, c, '3'};
  auto b = fused::make_tuple('4', d, 6u);

  auto x = fused::zip_view(a, b);
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  auto x0 = fused::get<0>(x);
  auto x1 = fused::get<1>(x);
  auto x2 = fused::get<2>(x);

  REQUIRE(&fused::get<0>(a) == &fused::get<0>(x0));
  REQUIRE(&fused::get<1>(a) == &fused::get<0>(x1));
  REQUIRE(&fused::get<2>(a) == &fused::get<0>(x2));
  REQUIRE(&fused::get<0>(b) == &fused::get<1>(x0));
  REQUIRE(&fused::get<1>(b) == &fused::get<1>(x1));
  REQUIRE(&fused::get<2>(b) == &fused::get<1>(x2));

  fused::get<0>(x0) = 7;
  fused::get<1>(x2) = 8u;

  REQUIRE(7 == fused::get<0>(a));
  REQUIRE(8u == fused::get<2>(b));
}
}
