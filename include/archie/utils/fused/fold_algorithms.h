#pragma once

#include <utility>
#include <archie/utils/fused/fold.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct accumulate_ {
        template <typename Tp, typename Up, typename... Vs>
        constexpr decltype(auto) operator()(Tp&& t, Up&& u, Vs&&... vs) const {
          return fused::make_fold([](auto&& a, auto&& b) {
            return std::forward<decltype(a)>(a) + std::forward<decltype(b)>(b);
          })(std::forward<Tp>(t), std::forward<Up>(u), std::forward<Vs>(vs)...);
        }
      };
      struct max_ {
        template <typename Tp, typename Up, typename... Vs>
        constexpr decltype(auto) operator()(Tp&& t, Up&& u, Vs&&... vs) const {
          return fused::make_fold([](auto&& a, auto&& b) {
            return b > a ? std::forward<decltype(b)>(b) : std::forward<decltype(a)>(a);
          })(std::forward<Tp>(t), std::forward<Up>(u), std::forward<Vs>(vs)...);
        }
      };
      struct min_ {
        template <typename Tp, typename Up, typename... Vs>
        constexpr decltype(auto) operator()(Tp&& t, Up&& u, Vs&&... vs) const {
          return fused::make_fold([](auto&& a, auto&& b) {
            return b < a ? std::forward<decltype(b)>(b) : std::forward<decltype(a)>(a);
          })(std::forward<Tp>(t), std::forward<Up>(u), std::forward<Vs>(vs)...);
        }
      };
      struct all_of_ {
        template <typename F, typename... Ts>
        constexpr decltype(auto) operator()(F&& f, Ts&&... ts) const {
          return operator()(std::forward<F>(f))(std::forward<Ts>(ts)...);
        }
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const {
          return fused::make_fold([f](auto s, auto x) { return s ? f(x) : false; }, true);
        }
      };
      struct any_of_ {
        template <typename F, typename... Ts>
        constexpr decltype(auto) operator()(F&& f, Ts&&... ts) const {
          return operator()(std::forward<F>(f))(std::forward<Ts>(ts)...);
        }
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const {
          return fused::make_fold([f](auto s, auto x) { return s | f(x); }, false);
        }
      };
    }
    constexpr auto const accumulate = detail::accumulate_{};
    constexpr auto const max = detail::max_{};
    constexpr auto const min = detail::min_{};
    constexpr auto const all_of = detail::all_of_{};
    constexpr auto const any_of = detail::any_of_{};
  }
}
}
