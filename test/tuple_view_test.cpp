#include <archie/utils/fused/tuple.h>
#include <functional>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp>
    struct value_view {
      using type = std::decay_t<Tp>;
      using reference = type&;
      using const_reference = type const&;
      value_view(type& ref) noexcept : ptr(&ref) {}
      value_view(type&&) = delete;
      value_view(value_view const&) noexcept = default;
      value_view& operator=(value_view&&) noexcept = default;
      value_view& operator=(value_view const&) noexcept = default;

      template <typename Up>
      value_view& operator=(Up&& u) {
        *ptr = u;
        return *this;
      }
      operator const_reference() const { return *ptr; }
      operator reference() { return *ptr; }

    private:
      type* ptr;
    };

    template <typename... Ts>
    using tuple_view = tuple<value_view<Ts>...>;

    template <typename... Ts>
    decltype(auto) make_view(Ts&... args) {
      return tuple<value_view<Ts>...>(args...);
    }

    template <std::size_t... idx, typename... Ts>
    decltype(auto) select(tuple<Ts...>& t) {
      return make_view(get<idx>(t)...);
    }
  }
}
}

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>

namespace {
namespace fused = archie::utils::fused;

struct tuple_view_test : ::testing::Test {
  fused::tuple<unsigned, double, char> t;
  void SetUp() { t = fused::make_tuple(1u, 2.0, '3'); }
};

TEST_F(tuple_view_test, canSelectAllNotChangingOrder) {
  auto view = fused::select<0, 1, 2>(t);

  ASSERT_EQ(fused::get<0>(t), fused::get<0>(view));
  ASSERT_EQ(fused::get<1>(t), fused::get<1>(view));
  ASSERT_EQ(fused::get<2>(t), fused::get<2>(view));

  fused::get<0>(view) = 4u;
  fused::get<1>(view) = 5.0;
  fused::get<2>(view) = '6';

  EXPECT_EQ(4u, fused::get<0>(t));
  EXPECT_EQ(5.0, fused::get<1>(t));
  EXPECT_EQ('6', fused::get<2>(t));
}

TEST_F(tuple_view_test, canSelectContinousSubsetWithSameOrder) {
  auto view1 = fused::select<0, 1>(t);
  auto view2 = fused::select<1, 2>(t);

  EXPECT_EQ(fused::get<0>(t), fused::get<0>(view1));
  EXPECT_EQ(fused::get<1>(t), fused::get<1>(view1));

  EXPECT_EQ(fused::get<1>(t), fused::get<0>(view2));
  EXPECT_EQ(fused::get<2>(t), fused::get<1>(view2));

  EXPECT_EQ(fused::get<1>(view1), fused::get<0>(view2));

  fused::get<1>(view1) = 4.0;

  EXPECT_EQ(4.0, fused::get<1>(t));
  EXPECT_EQ(4.0, fused::get<0>(view2));
}

TEST_F(tuple_view_test, canSelectNonContinousSubsetWithSameOrder) {
  auto view1 = fused::select<0, 2>(t);
  auto view2 = fused::select<1, 2>(t);

  EXPECT_EQ(fused::get<0>(t), fused::get<0>(view1));
  EXPECT_EQ(fused::get<2>(t), fused::get<1>(view1));

  EXPECT_EQ(fused::get<1>(t), fused::get<0>(view2));
  EXPECT_EQ(fused::get<2>(t), fused::get<1>(view2));

  EXPECT_EQ(fused::get<1>(view1), fused::get<1>(view2));

  fused::get<2>(t) = '4';

  EXPECT_EQ('4', fused::get<1>(view1));
  EXPECT_EQ('4', fused::get<1>(view2));
}

TEST_F(tuple_view_test, canSelectWithChangedOrder) {
  auto view1 = fused::select<1, 0>(t);
  auto view2 = fused::select<2, 1, 0>(t);

  EXPECT_EQ(fused::get<0>(t), fused::get<1>(view1));
  EXPECT_EQ(fused::get<1>(t), fused::get<0>(view1));

  EXPECT_EQ(fused::get<0>(t), fused::get<2>(view2));
  EXPECT_EQ(fused::get<1>(t), fused::get<1>(view2));
  EXPECT_EQ(fused::get<2>(t), fused::get<0>(view2));

  EXPECT_EQ(fused::get<0>(view1), fused::get<1>(view2));
  EXPECT_EQ(fused::get<1>(view1), fused::get<2>(view2));

  fused::get<0>(t) = 4u;
  fused::get<0>(view1) = 5.0;

  EXPECT_EQ(5.0, fused::get<1>(t));
  EXPECT_EQ(5.0, fused::get<1>(view2));
  EXPECT_EQ(4u, fused::get<1>(view1));
}

/*
TEST_F(tuple_view_test, canStoreItemManyTimes) {
  auto view = fused::select<0, 0, 1, 1, 2, 2>(t);

  EXPECT_EQ(&fused::get<0>(t), &fused::get<0>(view));
  EXPECT_EQ(&fused::get<0>(t), &fused::get<1>(view));

  EXPECT_EQ(&fused::get<1>(t), &fused::get<2>(view));
  EXPECT_EQ(&fused::get<1>(t), &fused::get<3>(view));

  EXPECT_EQ(&fused::get<2>(t), &fused::get<4>(view));
  EXPECT_EQ(&fused::get<2>(t), &fused::get<5>(view));
}

TEST_F(tuple_view_test, canUseValueView) {
  unsigned a = 3u;
  unsigned b = 1u;

  fused::value_view<unsigned> x = a;
  fused::value_view<unsigned> y = b;

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);

  auto z = std::move(x);
  x = std::move(y);
  y = std::move(z);

  EXPECT_EQ(&a, &y);
  EXPECT_EQ(&b, &x);

  std::swap(x, y);

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
}

TEST_F(tuple_view_test, canUseTupleView) {
  unsigned a = 3u;
  unsigned b = 1u;

  fused::tuple_view<unsigned> x(a);
  fused::tuple_view<unsigned> y(b);

  EXPECT_EQ(&a, &fused::get<0>(x));
  EXPECT_EQ(&b, &fused::get<0>(y));

  auto z = std::move(x);
  EXPECT_EQ(&a, &fused::get<0>(z));
  x = std::move(y);
  EXPECT_EQ(&b, &fused::get<0>(x));
  y = std::move(z);

  //EXPECT_EQ(&a, &fused::get<0>(y));
  //EXPECT_EQ(&b, &fused::get<0>(x));
}

TEST_F(tuple_view_test, canStoreViewInContainer) {
  std::vector<fused::tuple_view<char, unsigned>> vec;
  auto t2 = fused::make_tuple('4', 5u, 6.0);
  auto view1 = fused::select<2, 0>(t);
  auto view2 = fused::select<0, 1>(t2);

  vec.push_back(view1);
  vec.push_back(view2);
  vec.emplace_back(fused::select<2, 0>(t));

  std::sort(std::begin(vec), std::end(vec));
  ASSERT_EQ(3u, vec.size());

  EXPECT_EQ('3', fused::get<0>(vec[0]));
  EXPECT_EQ('3', fused::get<0>(vec[1]));
  EXPECT_EQ('4', fused::get<0>(vec[2]));
}
*/
}
