#include <archie/utils/number.h>

namespace archie {
namespace utils {

  template <typename... Ts>
  struct type_list {
    using size = Number<sizeof...(Ts)>;

    template <template <typename...> class Func>
    using apply = typename Func<Ts...>::type;

    template <template <typename> class Func>
    using transform = type_list<typename Func<Ts>::type...>;
  };
}
}

#include <gtest/gtest.h>
#include <tuple>
#include <type_traits>
#include <memory>

namespace {

template <unsigned I>
struct utype {};

using _0 = utype<0>;
using _1 = utype<1>;

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
  using type = list_::transform<uptr_>;
  static_assert(
      std::is_same<au::type_list<std::unique_ptr<_0>, std::unique_ptr<_1>>,
                   type>::value,
      "");
}

TEST_F(type_list_test, canTransformAndApply) {
  using type = list_::transform<uptr_>::apply<tuple_>;
  static_assert(
      std::is_same<std::tuple<std::unique_ptr<_0>, std::unique_ptr<_1>>,
                   type>::value,
      "");
}
}
