#pragma once

#include <archie/utils/meta/indexable.h>
#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct zip_ {
        template <typename... Ts, typename... Us>
        constexpr decltype(auto) operator()(tuple<Ts...> const& a,
                                            tuple<Us...> const& b) const {
          return make_tuple(make_tuple(get<Ts>(a), get<Us>(b))...);
        }
      };

      struct zip_view_ {
      private:
        template <std::size_t... ns>
        struct impl {
          template <typename Tp, typename Up>
          constexpr decltype(auto) operator()(Tp&& a, Up&& b) const {
            return fused::make_tuple(
                fused::tie(fused::get<ns>(a), fused::get<ns>(b))...);
          }
        };

      public:
        template <typename Tp, typename Up>
        constexpr decltype(auto) operator()(Tp&& a, Up&& b) const {
          return meta::indexable_t<
              impl, fused::tuple_size<std::remove_reference_t<Tp>>::value>{}(
              std::forward<Tp>(a), std::forward<Up>(b));
        }
      };
    }
    constexpr detail::zip_ zip{};
    constexpr detail::zip_view_ zip_view{};
  }
}
}
