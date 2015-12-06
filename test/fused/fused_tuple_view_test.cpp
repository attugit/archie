#include <archie/fused/tuple_view.hpp>
#include <algorithm>
#include <vector>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

TEST_CASE("canSelectAllNotChangingOrder") {
  auto t = fused::make_tuple(1u, 2.0, '3');
  auto view = fused::select<0, 1, 2>(t);

  REQUIRE(fused::get<0>(t) == fused::get<0>(view));
  REQUIRE(fused::get<1>(t) == fused::get<1>(view));
  REQUIRE(fused::get<2>(t) == fused::get<2>(view));

  fused::get<0>(view) = 4u;
  fused::get<1>(view) = 5.0;
  fused::get<2>(view) = '6';

  REQUIRE(4u == fused::get<0>(t));
  REQUIRE(5.0 == fused::get<1>(t));
  REQUIRE('6' == fused::get<2>(t));
}

TEST_CASE("canSelectContinousSubsetWithSameOrder") {
  auto t = fused::make_tuple(1u, 2.0, '3');
  auto view1 = fused::select<0, 1>(t);
  auto view2 = fused::select<1, 2>(t);

  REQUIRE(fused::get<0>(t) == fused::get<0>(view1));
  REQUIRE(fused::get<1>(t) == fused::get<1>(view1));

  REQUIRE(fused::get<1>(t) == fused::get<0>(view2));
  REQUIRE(fused::get<2>(t) == fused::get<1>(view2));

  REQUIRE(fused::get<1>(view1) == fused::get<0>(view2));

  fused::get<1>(view1) = 4.0;

  REQUIRE(4.0 == fused::get<1>(t));
  REQUIRE(4.0 == fused::get<0>(view2));
}

TEST_CASE("canSelectNonContinousSubsetWithSameOrder") {
  auto t = fused::make_tuple(1u, 2.0, '3');
  auto view1 = fused::select<0, 2>(t);
  auto view2 = fused::select<1, 2>(t);

  REQUIRE(fused::get<0>(t) == fused::get<0>(view1));
  REQUIRE(fused::get<2>(t) == fused::get<1>(view1));

  REQUIRE(fused::get<1>(t) == fused::get<0>(view2));
  REQUIRE(fused::get<2>(t) == fused::get<1>(view2));

  REQUIRE(fused::get<1>(view1) == fused::get<1>(view2));

  fused::get<2>(t) = '4';

  REQUIRE('4' == fused::get<1>(view1));
  REQUIRE('4' == fused::get<1>(view2));
}

TEST_CASE("canSelectWithChangedOrder") {
  auto t = fused::make_tuple(1u, 2.0, '3');
  auto view1 = fused::select<1, 0>(t);
  auto view2 = fused::select<2, 1, 0>(t);

  REQUIRE(fused::get<0>(t) == fused::get<1>(view1));
  REQUIRE(fused::get<1>(t) == fused::get<0>(view1));

  REQUIRE(fused::get<0>(t) == fused::get<2>(view2));
  REQUIRE(fused::get<1>(t) == fused::get<1>(view2));
  REQUIRE(fused::get<2>(t) == fused::get<0>(view2));

  REQUIRE(fused::get<0>(view1) == fused::get<1>(view2));
  REQUIRE(fused::get<1>(view1) == fused::get<2>(view2));

  fused::get<0>(t) = 4u;
  fused::get<0>(view1) = 5.0;

  REQUIRE(5.0 == fused::get<1>(t));
  REQUIRE(5.0 == fused::get<1>(view2));
  REQUIRE(4u == fused::get<1>(view1));
}

TEST_CASE("canStoreItemManyTimes") {
  auto t = fused::make_tuple(1u, 2.0, '3');
  auto view = fused::select<0, 0, 1, 1, 2, 2>(t);

  REQUIRE(fused::get<0>(t) == fused::get<0>(view));
  REQUIRE(fused::get<0>(t) == fused::get<1>(view));

  REQUIRE(fused::get<1>(t) == fused::get<2>(view));
  REQUIRE(fused::get<1>(t) == fused::get<3>(view));

  REQUIRE(fused::get<2>(t) == fused::get<4>(view));
  REQUIRE(fused::get<2>(t) == fused::get<5>(view));

  fused::get<2>(view) = 4.0;

  REQUIRE(4.0 == fused::get<1>(t));
  REQUIRE(4.0 == fused::get<3>(view));
}

TEST_CASE("canUseValueView") {
  unsigned a = 3u;
  unsigned b = 1u;

  fused::value_view<unsigned> x = a;
  fused::value_view<unsigned> y = b;

  REQUIRE(a == x);
  REQUIRE(b == y);

  auto z = std::move(x);
  x = std::move(y);
  y = std::move(z);

  REQUIRE(a != b);
  REQUIRE(a == y);
  REQUIRE(b == x);

  std::swap(x, y);

  REQUIRE(a != b);
  REQUIRE(a == x);
  REQUIRE(b == y);
}

TEST_CASE("canUseTupleView") {
  unsigned a = 3u;
  unsigned b = 1u;

  fused::tuple_view<unsigned> x(a);
  fused::tuple_view<unsigned> y(b);

  REQUIRE(a == fused::get<0>(x));
  REQUIRE(b == fused::get<0>(y));

  auto z = std::move(x);
  REQUIRE(3u == a);
  REQUIRE(1u == b);
  REQUIRE(a == fused::get<0>(z));

  x = std::move(y);
  REQUIRE(3u == a);
  REQUIRE(1u == b);
  REQUIRE(b == fused::get<0>(x));

  y = std::move(z);
  REQUIRE(3u == a);
  REQUIRE(1u == b);

  REQUIRE(a == fused::get<0>(y));
  REQUIRE(b == fused::get<0>(x));
}

TEST_CASE("canStoreViewInContainer") {
  auto t = fused::make_tuple(1u, 2.0, '3');
  std::vector<fused::tuple_view<char, unsigned>> vec;
  auto t2 = fused::make_tuple('4', 5u, 6.0);
  auto view1 = fused::select<2, 0>(t);
  auto view2 = fused::select<0, 1>(t2);

  vec.push_back(view1);
  vec.push_back(view2);
  vec.emplace_back(fused::select<2, 0>(t));

  std::sort(std::begin(vec), std::end(vec));
  REQUIRE(3u == vec.size());

  REQUIRE('3' == fused::get<0>(vec[0]));
  REQUIRE('3' == fused::get<0>(vec[1]));
  REQUIRE('4' == fused::get<0>(vec[2]));
}
}
