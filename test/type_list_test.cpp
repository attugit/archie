#include <archie/utils/number.h>

namespace archie {
namespace utils {

  template <typename... Ts>
  struct type_list {
    using size = Number<sizeof...(Ts)>;

    template <template <typename...> class Func>
    using apply = typename Func<Ts...>::type;
  };
}
}

#include <gtest/gtest.h>
#include <tuple>
#include <type_traits>

namespace {

template <unsigned I>
struct utype {};

template <typename... Ts>
struct tuple_ {
  using type = std::tuple<Ts...>;
};

namespace au = archie::utils;

struct type_list_test : testing::Test {};

TEST_F(type_list_test, canGetTypeListSize) {
  static_assert(au::type_list<>::size::value == 0, "");
  static_assert(au::type_list<utype<0>>::size::value == 1, "");
  static_assert(au::type_list<utype<0>, utype<0>>::size::value == 2, "");
  static_assert(au::type_list<utype<0>, utype<1>>::size::value == 2, "");
}

TEST_F(type_list_test, canApply) {
  using type = au::type_list<utype<0>, utype<1>>::apply<tuple_>;
  static_assert(std::is_same<std::tuple<utype<0>, utype<1>>, type>::value, "");
}
}
