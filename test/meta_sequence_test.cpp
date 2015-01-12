#include <archie/utils/meta/sequence.h>
#include <archie/utils/meta/transform.h>
#include <tuple>
#include <type_traits>
#include <memory>

template <unsigned I>
struct utype {};

using _0 = utype<0>;
using _1 = utype<1>;
using _2 = utype<2>;
using _3 = utype<3>;
using _4 = utype<4>;
using _5 = utype<5>;

template <typename... Ts>
struct tuple_ {
  using type = std::tuple<Ts...>;
};

template <typename Tp>
struct uptr_ {
  using type = std::unique_ptr<Tp>;
};

namespace au = archie::utils;

using au::meta::sequence;

static_assert(sequence<>::size::value == 0, "");
static_assert(sequence<_0>::size::value == 1, "");
static_assert(sequence<_0, _0>::size::value == 2, "");
static_assert(sequence<_0, _1>::size::value == 2, "");

using list_ = sequence<_0, _1>;

void canApply() {
  using type = list_::apply<tuple_>;
  static_assert(std::is_same<std::tuple<_0, _1>, type>::value, "");
}

void canTransform() {
  using type = list_::transform<uptr_>::type;
  static_assert(std::is_same<sequence<std::unique_ptr<_0>, std::unique_ptr<_1>>,
                             type>::value,
                "");
}

void canTransformAndApply() {
  using type = list_::transform_t<uptr_>::apply<tuple_>;
  static_assert(
      std::is_same<std::tuple<std::unique_ptr<_0>, std::unique_ptr<_1>>,
                   type>::value,
      "");
}

void canAppend() {
  using type = list_::append<_2, _3>;
  static_assert(std::is_same<sequence<_0, _1, _2, _3>, type>::value, "");
}

void canUseAt() {
  using type_0 = au::meta::at<0>::apply<_3, _2, _1, _0>::type;
  using type_1 = au::meta::at<1>::apply<_3, _2, _1, _0>::type;
  using type_2 = au::meta::at<2>::apply<_3, _2, _1, _0>::type;
  using type_3 = au::meta::at<3>::apply<_3, _2, _1, _0>::type;
  static_assert(std::is_same<_3, type_0>::value, "");
  static_assert(std::is_same<_2, type_1>::value, "");
  static_assert(std::is_same<_1, type_2>::value, "");
  static_assert(std::is_same<_0, type_3>::value, "");
}

void canUseAtT() {
  using type_0 = au::meta::at_t<0, _3, _2, _1, _0>;
  using type_1 = au::meta::at_t<1, _3, _2, _1, _0>;
  using type_2 = au::meta::at_t<2, _3, _2, _1, _0>;
  using type_3 = au::meta::at_t<3, _3, _2, _1, _0>;
  static_assert(std::is_same<_3, type_0>::value, "");
  static_assert(std::is_same<_2, type_1>::value, "");
  static_assert(std::is_same<_1, type_2>::value, "");
  static_assert(std::is_same<_0, type_3>::value, "");
}

void canUseTypeListAt() {
  using list = sequence<_3, _2, _1, _0>;
  static_assert(std::is_same<_3, list::at<0>::type>::value, "");
  static_assert(std::is_same<_2, list::at<1>::type>::value, "");
  static_assert(std::is_same<_1, list::at<2>::type>::value, "");
  static_assert(std::is_same<_0, list::at<3>::type>::value, "");
}

void canUseTypeListAtT() {
  using list = sequence<_3, _2, _1, _0>;
  static_assert(std::is_same<_3, list::at_t<0>>::value, "");
  static_assert(std::is_same<_2, list::at_t<1>>::value, "");
  static_assert(std::is_same<_1, list::at_t<2>>::value, "");
  static_assert(std::is_same<_0, list::at_t<3>>::value, "");
}

void canTransformStandalone() {
  using uptrs = au::meta::transform_t<uptr_, _0, _1>;
  static_assert(
      std::is_same<
          au::meta::type_list<std::unique_ptr<_0>, std::unique_ptr<_1>>,
          uptrs>::value,
      "");
}

void canGetIndexOfListItem() {
  using list = sequence<_3, _2, _1, _0>;
  static_assert(list::index_of<_0>::value == 3, "");
  static_assert(list::index_of<_1>::value == 2, "");
  static_assert(list::index_of<_2>::value == 1, "");
  static_assert(list::index_of<_3>::value == 0, "");
}

void canCheckIfListContainsItem() {
  using list = sequence<_3, _2, _1, _0, _0>;
  static_assert(list::contains<_0>::value, "");
  static_assert(list::contains<_1>::value, "");
  static_assert(list::contains<_2>::value, "");
  static_assert(list::contains<_3>::value, "");
  static_assert(!list::contains<_4>::value, "");
  static_assert(!list::contains<_5>::value, "");
}

void canFindType() {
  using list = sequence<_3, _2, _1, _0, _0, _1, _2, _3>;
  static_assert(list::find<_3>::value == 0, "");
  static_assert(list::find<_2>::value == 1, "");
  static_assert(list::find<_1>::value == 2, "");
  static_assert(list::find<_0>::value == 3, "");
  static_assert(list::find<_4>::value == list::size::value, "");
  static_assert(list::find<_5>::value == list::size::value, "");
}

int main() {
  canApply();
  canTransform();
  canTransformAndApply();
  canAppend();
  canUseAt();
  canUseAtT();
  canUseTypeListAt();
  canUseTypeListAtT();
  canTransformStandalone();
  canGetIndexOfListItem();
  canCheckIfListContainsItem();
  canFindType();
  return 0;
}
