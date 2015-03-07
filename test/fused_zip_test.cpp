#include <archie/utils/fused/zip.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canZipTwoTuples() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip(a, b);

  EXPECT_EQ(3u, fused::tuple_size<decltype(x)>::value);
  EXPECT_EQ(fused::make_tuple(1, '4'), fused::get<0>(x));
  EXPECT_EQ(fused::make_tuple(2u, 5.0), fused::get<1>(x));
  EXPECT_EQ(fused::make_tuple('3', 6u), fused::get<2>(x));
}

void canZipManyTuples() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple(4u, '5', 6);
  auto c = fused::make_tuple('7', 8, 9u);
  auto x = fused::zip(a, b, c);

  EXPECT_EQ(3u, fused::tuple_size<decltype(x)>::value);
  EXPECT_EQ(fused::make_tuple(1, 4u, '7'), fused::get<0>(x));
  EXPECT_EQ(fused::make_tuple(2u, '5', 8), fused::get<1>(x));
  EXPECT_EQ(fused::make_tuple('3', 6, 9u), fused::get<2>(x));
}

void canZipViewTwoTuples() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip_view(a, b);

  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  auto const& x0 = fused::get<0>(x);
  auto const& x1 = fused::get<1>(x);
  auto const& x2 = fused::get<2>(x);

  static_assert(
      std::is_same<decltype(x0), fused::tuple<int&, char&> const&>::value, "");
  static_assert(std::is_same<decltype(x1),
                             fused::tuple<unsigned&, double&> const&>::value,
                "");
  static_assert(
      std::is_same<decltype(x2), fused::tuple<char&, unsigned&> const&>::value,
      "");

  EXPECT_EQ(&fused::get<0>(a), &fused::get<0>(x0));
  EXPECT_EQ(&fused::get<1>(a), &fused::get<0>(x1));
  EXPECT_EQ(&fused::get<2>(a), &fused::get<0>(x2));

  EXPECT_EQ(&fused::get<0>(b), &fused::get<1>(x0));
  EXPECT_EQ(&fused::get<1>(b), &fused::get<1>(x1));
  EXPECT_EQ(&fused::get<2>(b), &fused::get<1>(x2));

  fused::get<0>(x1) = 7u;
  EXPECT_EQ(7u, fused::get<1>(a));
  fused::get<1>(x2) = 8u;
  EXPECT_EQ(8u, fused::get<2>(b));
}

void canZipViewManyTuples() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple(4u, '5', 6.0);
  auto c = fused::make_tuple('7', 8.0, 9u);
  auto x = fused::zip_view(a, b, c);

  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  auto const& x0 = fused::get<0>(x);
  auto const& x1 = fused::get<1>(x);
  auto const& x2 = fused::get<2>(x);

  static_assert(
      std::is_same<decltype(x0),
                   fused::tuple<int&, unsigned&, char&> const&>::value,
      "");
  static_assert(
      std::is_same<decltype(x1),
                   fused::tuple<unsigned&, char&, double&> const&>::value,
      "");
  static_assert(
      std::is_same<decltype(x2),
                   fused::tuple<char&, double&, unsigned&> const&>::value,
      "");

  EXPECT_EQ(&fused::get<0>(a), &fused::get<0>(x0));
  EXPECT_EQ(&fused::get<1>(a), &fused::get<0>(x1));
  EXPECT_EQ(&fused::get<2>(a), &fused::get<0>(x2));

  EXPECT_EQ(&fused::get<0>(b), &fused::get<1>(x0));
  EXPECT_EQ(&fused::get<1>(b), &fused::get<1>(x1));
  EXPECT_EQ(&fused::get<2>(b), &fused::get<1>(x2));

  EXPECT_EQ(&fused::get<0>(c), &fused::get<2>(x0));
  EXPECT_EQ(&fused::get<1>(c), &fused::get<2>(x1));
  EXPECT_EQ(&fused::get<2>(c), &fused::get<2>(x2));

  fused::get<0>(x1) = 10u;
  EXPECT_EQ(10u, fused::get<1>(a));
  fused::get<1>(x2) = 11.0;
  EXPECT_EQ(11.0, fused::get<2>(b));
}

void canZipViewTwoConstTuples() {
  auto const a = fused::make_tuple(1, 2u, '3');
  auto const b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip_view(a, b);

  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  auto x0 = fused::get<0>(x);
  auto x1 = fused::get<1>(x);
  auto x2 = fused::get<2>(x);

  EXPECT_EQ(&fused::get<0>(a), &fused::get<0>(x0));
  EXPECT_EQ(&fused::get<1>(a), &fused::get<0>(x1));
  EXPECT_EQ(&fused::get<2>(a), &fused::get<0>(x2));
  EXPECT_EQ(&fused::get<0>(b), &fused::get<1>(x0));
  EXPECT_EQ(&fused::get<1>(b), &fused::get<1>(x1));
  EXPECT_EQ(&fused::get<2>(b), &fused::get<1>(x2));
}

void canZipViewTwoTuplesWithConsts() {
  auto const c = 2u;
  auto const d = 5.0;
  auto a = fused::tuple<int, const unsigned, char>{1, c, '3'};
  auto b = fused::make_tuple('4', d, 6u);

  auto x = fused::zip_view(a, b);
  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  auto x0 = fused::get<0>(x);
  auto x1 = fused::get<1>(x);
  auto x2 = fused::get<2>(x);

  EXPECT_EQ(&fused::get<0>(a), &fused::get<0>(x0));
  EXPECT_EQ(&fused::get<1>(a), &fused::get<0>(x1));
  EXPECT_EQ(&fused::get<2>(a), &fused::get<0>(x2));
  EXPECT_EQ(&fused::get<0>(b), &fused::get<1>(x0));
  EXPECT_EQ(&fused::get<1>(b), &fused::get<1>(x1));
  EXPECT_EQ(&fused::get<2>(b), &fused::get<1>(x2));

  fused::get<0>(x0) = 7;
  fused::get<1>(x2) = 8u;

  EXPECT_EQ(7, fused::get<0>(a));
  EXPECT_EQ(8u, fused::get<2>(b));
}

int main() {
  canZipTwoTuples();
  canZipManyTuples();
  canZipViewTwoTuples();
  canZipViewManyTuples();
  canZipViewTwoConstTuples();
  canZipViewTwoTuplesWithConsts();
  return 0;
}
