#include <archie/utils/number.h>
#include <archie/utils/eat.h>

namespace archie {
namespace utils {

  template <typename First, typename... Rest>
  struct get_front {
    using type = First;
  };

  template <template <typename...> class...>
  struct compose;

  template <template <typename...> class F>
  struct compose<F> {
    template <typename... Xs>
    using apply = typename F<Xs...>::type;
  };

  template <template <typename...> class F, template <typename...> class... Gs>
  struct compose<F, Gs...> {
    template <typename... Xs>
    using apply =
        typename F<typename compose<Gs...>::template apply<Xs...>>::type;
  };

  template <std::size_t n, typename = std::make_index_sequence<n>>
  struct at;

  template <std::size_t n, std::size_t... ignore>
  struct at<n, std::index_sequence<ignore...>> {
    template <typename... Ts>
    struct skip {
    private:
      template <typename Up>
      static constexpr Up match(eat<Ts>..., Up, ...) noexcept;

    public:
      template <typename... Us>
      using from = decltype(match(std::declval<Us>()...));
    };

    template <typename... Ts>
    using from = typename skip<Number<ignore>...>::template from<Ts...>;
  };

  template <typename... Ts>
  struct type_list {
    using size = Number<sizeof...(Ts)>;

    template <template <typename...> class Func>
    using apply = typename Func<Ts...>::type;

    template <template <typename> class Func>
    using transform = type_list<typename Func<Ts>::type...>;

    template <typename... Us>
    using append = type_list<Ts..., Us...>;

    template <std::size_t I>
    using at = typename at<I>::template from<Ts...>;
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

TEST_F(type_list_test, canAppend) {
  using type = list_::append<_2, _3>;
  static_assert(std::is_same<au::type_list<_0, _1, _2, _3>, type>::value, "");
}

TEST_F(type_list_test, canGetFront) {
  using type = list_::apply<au::get_front>;
  static_assert(std::is_same<_0, type>::value, "");
}

template <typename... Xs>
using foo = au::compose<uptr_>::apply<Xs...>;

TEST_F(type_list_test, canComposeSingleFunction) {
  using type = foo<_0>;
  static_assert(std::is_same<std::unique_ptr<_0>, type>::value, "");
}

template <typename... Xs>
struct size {
  using type = au::Number<sizeof...(Xs)>;
};

template <typename... Xs>
using goo = au::compose<uptr_, size>::apply<Xs...>;

TEST_F(type_list_test, canComposeManyFunctions) {
  using type = goo<_0, _1>;
  static_assert(std::is_same<std::unique_ptr<au::Number<2>>, type>::value, "");
}

TEST_F(type_list_test, canUseAt) {
  using type_0 = au::at<0>::from<_3, _2, _1, _0>;
  using type_1 = au::at<1>::from<_3, _2, _1, _0>;
  using type_2 = au::at<2>::from<_3, _2, _1, _0>;
  using type_3 = au::at<3>::from<_3, _2, _1, _0>;
  static_assert(std::is_same<_3, type_0>::value, "");
  static_assert(std::is_same<_2, type_1>::value, "");
  static_assert(std::is_same<_1, type_2>::value, "");
  static_assert(std::is_same<_0, type_3>::value, "");
}

TEST_F(type_list_test, canUseTypeListAt) {
  using list = au::type_list<_3, _2, _1, _0>;
  static_assert(std::is_same<_3, list::at<0>>::value, "");
  static_assert(std::is_same<_2, list::at<1>>::value, "");
  static_assert(std::is_same<_1, list::at<2>>::value, "");
  static_assert(std::is_same<_0, list::at<3>>::value, "");
}
}
