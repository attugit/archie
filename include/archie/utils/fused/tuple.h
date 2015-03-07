#pragma once
#include <config.h>

#if defined(USE_ARCHIE_TUPLE)
#include <archie/utils/fused/tuple_archie.h>
#elif defined(USE_STD_TUPLE)
#include <archie/utils/fused/tuple_std.h>
#endif

#include <utility>
#include <archie/utils/fused/type_tag.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct tuple_size_n_ {
        template <typename Tp>
        constexpr decltype(auto) operator()(type_tag<Tp>) const {
#if defined(HAS_VARIABLE_TEMPLATES)
          constexpr auto const& x = fused::id<std::decay_t<Tp>>;
#else
          constexpr auto const& x = fused::id<std::decay_t<Tp>>::value;
#endif
          return impl(x);
        }
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp const&) const {
#if defined(HAS_VARIABLE_TEMPLATES)
          constexpr auto const& x = fused::id<Tp>;
#else
          constexpr auto const& x = fused::id<Tp>::value;
#endif
          return impl(x);
        }

      private:
        template <typename... Ts>
        constexpr decltype(auto) impl(type_tag<fused::tuple<Ts...>>) const {
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