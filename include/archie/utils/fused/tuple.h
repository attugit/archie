#pragma once
#include <config.h>

#if defined(USE_ARCHIE_TUPLE)
#include <archie/utils/fused/tuple_archie.h>
#elif defined(USE_STD_TUPLE)
#include <archie/utils/fused/tuple_std.h>
#endif

#include <utility>
#include <archie/utils/meta/identity.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct tuple_size_n_ {
        template <typename Tp>
        constexpr decltype(auto) operator()(meta::identity<Tp>) const {
          return impl(meta::identity<std::decay_t<Tp>>{});
        }
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp const&) const {
          return impl(meta::identity<Tp>{});
        }

      private:
        template <typename... Ts>
        constexpr decltype(auto) impl(
            meta::identity<fused::tuple<Ts...>>) const {
          return sizeof...(Ts);
        }
      };
    }
    constexpr auto const make_tuple = detail::make_tuple_{};
    constexpr auto const tie = detail::tie_{};
    constexpr auto const tuple_size_n = detail::tuple_size_n_{};
  }
}
}