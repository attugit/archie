#include <cstdint>
#include <utility>

#include <archie/utils/variadic.h>

namespace archie {
namespace utils {

  /*
    template <typename... Args>
    decltype(auto) make_capture(Args... args) {
      return [args...](auto&& func) -> decltype(auto) {
        return std::forward<decltype(func)>(func)(args...);
      };
    }

    template <typename... Tp>
    struct Tuple {
      using Capture = decltype(make_capture(std::declval<Tp>()...));
      Capture capture;

      template <typename... Up>
      explicit Tuple(Up&&... u)
          : capture(make_capture(std::forward<Up>(u)...)) {}

      Tuple() : Tuple(Tp {}...) {}
    };
    struct eat {
      template <typename... Up>
      constexpr eat(Up&&...) {}
    };

    template <std::size_t n, typename = std::make_index_sequence<n>>
    struct get_impl;

    template <std::size_t n, std::size_t... ignore>
    struct get_impl<n, std::index_sequence<ignore...>> {
      template <typename Nth, typename... Rest>
      constexpr decltype(auto) operator()(decltype(ignore, eat{})..., Nth&& nth,
                                          Rest&&...) const {
        return std::forward<Nth>(nth);
      }
    };

    template <std::size_t n, typename TypeSet>
    decltype(auto) get(TypeSet&& ts) {
      return std::forward<TypeSet>(ts).capture(get_impl<n>{});
    }
  */
}
}

#include <gtest/gtest.h>
namespace au = archie::utils;

namespace {

struct Aqq {
  Aqq() = delete;
  Aqq(int) {}
};

struct variadic_test : ::testing::Test {};

TEST_F(variadic_test, canCreateTuple) {
  auto t0 = au::Tuple<char, int>();
  auto t1 = au::Tuple<char>('d');
  EXPECT_EQ(2u, t0.size());
  EXPECT_EQ(1u, t1.size());
}

TEST_F(variadic_test, for_each) {
  int idx = 0;
  au::for_each([&idx](auto&&) { ++idx; }, 7, 'c', std::string{});
  EXPECT_EQ(3, idx);
}
}
