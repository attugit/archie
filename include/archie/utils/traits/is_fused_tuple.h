#pragma once

#include <archie/utils/meta/has_member.h>
#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      struct inspect_fused_tuple_helper {
        template <typename... Ts>
        static void use(fused::tuple<Ts...>&&);
        template <typename... Ts>
        static void use(fused::tuple<Ts...>&);
        template <typename... Ts>
        static void use(fused::tuple<Ts...> const&);
      };

      template <typename Tp>
      using inspect_fused_tuple =
          decltype(inspect_fused_tuple_helper::use(std::declval<Tp>()));
    }
    template <typename Tp>
    using is_fused_tuple =
        meta::has_member<detail::inspect_fused_tuple, meta::type_list<Tp>>;
  }
}
}
