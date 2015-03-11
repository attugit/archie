#pragma once

#include <utility>
#include <functional>
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
      private:
        template <typename F>
        struct impl {
          F const f;
          constexpr impl(F const& f) : f(f) {}
          template <typename Tp, typename Up>
          constexpr decltype(auto) operator()(Tp const& a, Up const& b) const {
            return f(a, b) ? a : b;
          }
        };

      public:
        template <typename F, typename Tp>
        constexpr decltype(auto) operator()(F const& f, Tp&& t) const {
          return fused::make_fold(impl<F>{f}, std::forward<Tp>(t));
        }
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return fused::make_fold(impl<F>{f});
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
      struct is_sorted_ {
      private:
        template <typename F>
        struct impl {
          F const f;
          impl(F const f) : f(f){};
          template <typename Tp, typename Up, typename Vp, typename... Rs>
          constexpr bool operator()(Tp const t, Up const& u, Vp const& v, Rs const&...) const {
            return t ? f(u, v) : false;
          }
          template <typename Tp, typename Up>
          constexpr bool operator()(Tp const& t, Up) const {
            return t;
          }
        };

      public:
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return [g = impl<F>{f}](auto&&... xs) {
            return fused::greedy_fold(g, true, std::forward<decltype(xs)>(xs)...);
          };
        }
      };
    }
    constexpr auto const accumulate = detail::accumulate_{};
    constexpr auto const extremum = detail::extremum_{};
    constexpr auto const max = extremum(std::greater_equal<>{});
    constexpr auto const min = extremum(std::less_equal<>{});
    constexpr auto const all_of = detail::all_of_{};
    constexpr auto const any_of = detail::any_of_{};
    constexpr auto const none_of = detail::none_of_{};
    constexpr auto const count_if = detail::count_if_{};
    constexpr auto const is_sorted = detail::is_sorted_{};
  }
}
}
