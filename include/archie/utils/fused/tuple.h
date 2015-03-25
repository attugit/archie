#pragma once

#include <utility>
#if defined(USE_ARCHIE_TUPLE)
#include <archie/utils/fused/tuple_archie.h>
#elif defined(USE_STD_TUPLE)
#include <archie/utils/fused/tuple_std.h>
#endif

#include <archie/utils/fused/type_tag.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct tuple_size_ {
        template <typename Tp>
        constexpr decltype(auto) operator()(type_tag<Tp>) const {
          return impl(VARTEMPL(fused::id, std::decay_t<Tp>));
        }
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp const&) const {
          return impl(VARTEMPL(fused::id, Tp));
        }

      private:
        template <typename... Ts>
        constexpr decltype(auto) impl(type_tag<fused::tuple<Ts...>>) const {
          return sizeof...(Ts);
        }
      };

      template <std::size_t n>
      struct at_ {
        template <typename Up>
        constexpr decltype(auto) operator()(Up&& up) const {
          return fused::get<n>(std::forward<Up>(up));
        }
      };

      template <typename Tp>
      struct extract_ {
        template <typename Up>
        constexpr decltype(auto) operator()(Up&& up) const {
          return fused::get<Tp>(std::forward<Up>(up));
        }
      };
    }

    template <std::size_t I, typename Tp>
    using tuple_element_t = meta::eval<fused::tuple_element<I, Tp>>;

    constexpr auto const make_tuple = detail::make_tuple_{};
    constexpr auto const tie = detail::tie_{};
    constexpr auto const tuple_size = detail::tuple_size_{};

    DECL_VARTEMPL(at, detail::at_, std::size_t);
    DECL_VARTEMPL(extract, detail::extract_, typename);
  }
}
}
