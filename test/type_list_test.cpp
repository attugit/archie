#include <archie/utils/type_list.h>

#include <gtest/gtest.h>
#include <tuple>
#include <type_traits>
#include <memory>

namespace {

template <unsigned I>
struct utype {};

using _0 = utype<0>;
using _1 = utype<1>;
using _2 = utype<2>;
using _3 = utype<3>;

template <typename... Ts>
struct tuple_ {
  using type = std::tuple<Ts...>;
};

template <typename Tp>
struct uptr_ {
  using type = std::unique_ptr<Tp>;
};

namespace au = archie::utils;

struct type_list_test : testing::Test {};

TEST_F(type_list_test, canGetTypeListSize) {
  static_assert(au::type_list<>::size::value == 0, "");
  static_assert(au::type_list<_0>::size::value == 1, "");
  static_assert(au::type_list<_0, _0>::size::value == 2, "");
  static_assert(au::type_list<_0, _1>::size::value == 2, "");
}

using list_ = au::type_list<_0, _1>;

TEST_F(type_list_test, canApply) {
  using type = list_::apply<tuple_>;
  static_assert(std::is_same<std::tuple<_0, _1>, type>::value, "");
}

TEST_F(type_list_test, canTransform) {
  using type = list_::transform<uptr_>::type;
  static_assert(
      std::is_same<au::type_list<std::unique_ptr<_0>, std::unique_ptr<_1>>,
                   type>::value,
      "");
}

TEST_F(type_list_test, canTransformAndApply) {
  using type = list_::transform_t<uptr_>::apply<tuple_>;
  static_assert(
      std::is_same<std::tuple<std::unique_ptr<_0>, std::unique_ptr<_1>>,
                   type>::value,
      "");
}

TEST_F(type_list_test, canAppend) {
  using type = list_::append<_2, _3>;
  static_assert(std::is_same<au::type_list<_0, _1, _2, _3>, type>::value, "");
}

TEST_F(type_list_test, canGetFront) {
  using type = list_::apply<au::get_front>;
  static_assert(std::is_same<_0, type>::value, "");
}

TEST_F(type_list_test, canUseAt) {
  using type_0 = au::meta::at<0>::apply<_3, _2, _1, _0>::type;
  using type_1 = au::meta::at<1>::apply<_3, _2, _1, _0>::type;
  using type_2 = au::meta::at<2>::apply<_3, _2, _1, _0>::type;
  using type_3 = au::meta::at<3>::apply<_3, _2, _1, _0>::type;
  static_assert(std::is_same<_3, type_0>::value, "");
  static_assert(std::is_same<_2, type_1>::value, "");
  static_assert(std::is_same<_1, type_2>::value, "");
  static_assert(std::is_same<_0, type_3>::value, "");
}

TEST_F(type_list_test, canUseAtT) {
  using type_0 = au::meta::at_t<0, _3, _2, _1, _0>;
  using type_1 = au::meta::at_t<1, _3, _2, _1, _0>;
  using type_2 = au::meta::at_t<2, _3, _2, _1, _0>;
  using type_3 = au::meta::at_t<3, _3, _2, _1, _0>;
  static_assert(std::is_same<_3, type_0>::value, "");
  static_assert(std::is_same<_2, type_1>::value, "");
  static_assert(std::is_same<_1, type_2>::value, "");
  static_assert(std::is_same<_0, type_3>::value, "");
}

TEST_F(type_list_test, canUseTypeListAt) {
  using list = au::type_list<_3, _2, _1, _0>;
  static_assert(std::is_same<_3, list::at<0>::type>::value, "");
  static_assert(std::is_same<_2, list::at<1>::type>::value, "");
  static_assert(std::is_same<_1, list::at<2>::type>::value, "");
  static_assert(std::is_same<_0, list::at<3>::type>::value, "");
}

TEST_F(type_list_test, canUseTypeListAtT) {
  using list = au::type_list<_3, _2, _1, _0>;
  static_assert(std::is_same<_3, list::at_t<0>>::value, "");
  static_assert(std::is_same<_2, list::at_t<1>>::value, "");
  static_assert(std::is_same<_1, list::at_t<2>>::value, "");
  static_assert(std::is_same<_0, list::at_t<3>>::value, "");
}

struct meta_test : ::testing::Test {};

TEST_F(meta_test, canTransform) {
  using uptrs = au::meta::transform_t<uptr_, _0, _1>;
  static_assert(
      std::is_same<au::type_list<std::unique_ptr<_0>, std::unique_ptr<_1>>,
                   uptrs>::value,
      "");
}
}
