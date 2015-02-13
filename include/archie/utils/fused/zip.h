#pragma once

#include <archie/utils/meta/indexable.h>
#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct zip_impl {
      private:
        template <std::size_t... ns>
        struct impl {
          template <typename F, typename Tp, typename Up>
          constexpr decltype(auto) operator()(F&& f, Tp&& a, Up&& b) const {
            return fused::make_tuple(
                std::forward<F>(f)(fused::get<ns>(a), fused::get<ns>(b))...);
          }
        };
      public:
        template <typename F, typename Tp, typename Up>
        constexpr decltype(auto) operator()(F&& f, Tp&& a, Up&& b) const {
          return meta::indexable_t<
              impl, fused::tuple_size<std::remove_reference_t<Tp>>::value>{}(
              std::forward<F>(f), std::forward<Tp>(a), std::forward<Up>(b));
        }
      };

      struct zip_ : private zip_impl {
        template <typename Tp, typename Up>
        constexpr decltype(auto) operator()(Tp&& a, Up&& b) const {
          return zip_impl::operator()(fused::make_tuple, std::forward<Tp>(a),
                                      std::forward<Up>(b));
        }
      };

      struct zip_view_ : private zip_impl {
        template <typename Tp, typename Up>
        constexpr decltype(auto) operator()(Tp&& a, Up&& b) const {
          return zip_impl::operator()(fused::tie, std::forward<Tp>(a),
                                      std::forward<Up>(b));
        }
      };
    }
    constexpr detail::zip_ zip{};
    constexpr detail::zip_view_ zip_view{};
  }
}
}
