#pragma once

#include <utility>
#include <archie/fused/fold.hpp>

namespace archie::fused
{
  constexpr auto const reduce = fused::make_fold;

  constexpr auto const extremum = [](auto f, auto&&... init) {
    return reduce(
        [f](auto&& s, auto&& x) -> decltype(auto) {
          return f(std::forward<decltype(s)>(s), std::forward<decltype(x)>(x))
                     ? std::forward<decltype(s)>(s)
                     : std::forward<decltype(x)>(x);
        },
        std::forward<decltype(init)>(init)...);
  };

  constexpr auto const max = [](auto const& t, auto const&... u) {
    auto const* ptr = &t;
    ((ptr = &std::max(*ptr, u)), ...);
    return *ptr;
  };

  constexpr auto const min = [](auto const& t, auto const&... u) {
    auto const* ptr = &t;
    ((ptr = &std::min(*ptr, u)), ...);
    return *ptr;
  };

  constexpr auto const all_of = [](auto f) {
    return reduce([f](bool s, auto&& x) { return s & f(std::forward<decltype(x)>(x)); }, true);
  };

  constexpr auto const any_of = [](auto f) {
    return reduce([f](bool s, auto&& x) { return s | f(std::forward<decltype(x)>(x)); }, false);
  };

  constexpr auto const none_of = [](auto f) {
    return reduce([f](bool s, auto&& x) { return s & !f(std::forward<decltype(x)>(x)); }, true);
  };

  constexpr auto const count_if = [](auto f) {
    return reduce([f](unsigned s, auto&& x) { return s + (!!f(std::forward<decltype(x)>(x))); },
                  0u);
  };

  constexpr auto const is_sorted = [](auto const f) {
    return fused::overload(
        [] { return true; }, [](auto const&) { return true; },
        [f](auto const& x, auto const&... y) {
          using item_t =
              std::common_type_t<std::decay_t<decltype(x)>, std::decay_t<decltype(y)>...>;
          using state_t =
              std::pair<decltype(f(std::declval<item_t>(), std::declval<item_t>())), item_t const&>;
          auto const pred = [f](auto const& s, auto const& a) {
            auto const & [ cond, b ] = s;
            return state_t(cond ? f(b, a) : cond, a);
          };
          return reduce(pred, state_t(true, x))(y...).first;
        });
  };
}
