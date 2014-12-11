#ifndef ARCHIE_UTILS_INSPECT_H_INCLUDED
#define ARCHIE_UTILS_INSPECT_H_INCLUDED

#include <archie/utils/meta/has_member.h>

namespace archie {
namespace utils {
  namespace detail {
    template <typename Tp>
    using InspectReserve = decltype(
        std::declval<Tp>().reserve(std::declval<typename Tp::size_type>()));

    template <typename Tp>
    using InspectCopyAssignable =
        decltype(std::declval<Tp&>() = std::declval<Tp const&>());

    template <typename Tp>
    using InspectDataProperty = decltype(std::declval<Tp>().data);

    template <typename F, typename... Ts>
    using InspectCallable = decltype(std::declval<F>()(std::declval<Ts>()...));
  }
  template <typename Tp>
  using has_reserve =
      meta::has_member<detail::InspectReserve, meta::type_list<Tp>>;

  template <typename Tp>
  using is_copy_assignable =
      meta::has_member<detail::InspectCopyAssignable, meta::type_list<Tp>>;

  template <typename Tp>
  using ValueType = typename Tp::value_type;

  template <typename Tp>
  using has_value_type = meta::has_member<ValueType, meta::type_list<Tp>>;

  template <typename Tp>
  using has_data_property =
      meta::has_member<detail::InspectDataProperty, meta::type_list<Tp>>;

  template <typename F, typename... Ts>
  using is_callable =
      meta::has_member<detail::InspectCallable, meta::type_list<F, Ts...>>;
}
}

#endif
