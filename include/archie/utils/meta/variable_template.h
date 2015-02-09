#pragma once

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    struct variable_template_type {
      static constexpr Tp value{};
    };
    template <typename Tp>
    constexpr Tp variable_template_type<Tp>::value;

    template <typename Tp, Tp V, template <Tp> class Up>
    struct variable_template_non_type {
      static constexpr Up<V> value{};
    };
    template <typename Tp, Tp V, template <Tp> class Up>
    constexpr Up<V> variable_template_non_type<Tp, V, Up>::value;
  }
}
}
