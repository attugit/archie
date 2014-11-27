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
        decltype(std::declval<Tp>() = std::declval<Tp const&>());

    template <typename Tp>
    using InspectDataProperty = decltype(std::declval<Tp>().data);
  }
  template <typename Tp>
  using HasReserve = meta::has_member<detail::InspectReserve, Tp>;

  template <typename Tp>
  using IsCopyAssignable = meta::has_member<detail::InspectCopyAssignable, Tp>;

  template <typename Tp>
  using ValueType = typename Tp::value_type;

  template <typename Tp>
  using VariadicType = typename std::remove_reference_t<Tp>::variadic;

  template <typename Tp>
  using has_value_type = meta::has_member<ValueType, Tp>;

  template <typename Tp>
  using HasVariadicType = meta::has_member<VariadicType, Tp>;

  template <typename Tp>
  using HasDataProperty = meta::has_member<detail::InspectDataProperty, Tp>;
}
}

#endif
