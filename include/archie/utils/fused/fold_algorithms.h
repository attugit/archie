#pragma once

#include <utility>
#include <archie/utils/fused/fold.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct accumulate_ {
        template <typename F, typename Tp>
        constexpr decltype(auto) operator()(F const& f, Tp&& t) const {
          return fused::make_fold([f](auto&& a, auto&& b) {
                                    return f(std::forward<decltype(a)>(a),
                                             std::forward<decltype(b)>(b));
                                  },
                                  std::forward<Tp>(t));
        }
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return fused::make_fold([f](auto&& a, auto&& b) {
            return f(std::forward<decltype(a)>(a), std::forward<decltype(b)>(b));
          });
        }
      };
      struct extremum_ {
        template <typename F, typename Tp>
        constexpr decltype(auto) operator()(F const& f, Tp&& t) const {
          return fused::make_fold([f](auto const& a, auto const& b) { return f(a, b) ? a : b; },
                                  std::forward<Tp>(t));
        }
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return fused::make_fold([f](auto const& a, auto const& b) { return f(a, b) ? a : b; });
        }
      };
      struct all_of_ {
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return fused::make_fold([f](auto const& s, auto const& x) { return s & f(x); }, true);
        }
      };
      struct any_of_ {
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return fused::make_fold([f](auto const& s, auto const& x) { return s | f(x); }, false);
        }
      };
      struct none_of_ {
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return fused::make_fold([f](auto const& s, auto const& x) { return s & !f(x); }, true);
        }
      };
      struct count_if_ {
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return fused::make_fold([f](auto const& s, auto const& x) { return f(x) ? s + 1 : s; },
                                  0u);
        }
      };
    }
    constexpr auto const accumulate = detail::accumulate_{};
    constexpr auto const extremum = detail::extremum_{};
    constexpr auto const all_of = detail::all_of_{};
    constexpr auto const any_of = detail::any_of_{};
    constexpr auto const none_of = detail::none_of_{};
    constexpr auto const count_if = detail::count_if_{};
  }
}
}
