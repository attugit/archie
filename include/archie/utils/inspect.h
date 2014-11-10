#ifndef ARCHIE_UTILS_INSPECT_H_INCLUDED
#define ARCHIE_UTILS_INSPECT_H_INCLUDED

#include <archie/utils/has_member.h>

namespace archie {
namespace utils {

  template <typename Tp>
  using InspectReserve =
      decltype(std::declval<Tp>().reserve(std::declval<int>()));

  template <typename Tp>
  using InspectCopyAssignable =
      decltype(std::declval<Tp>() = std::declval<Tp const&>());

  template <typename Tp>
  using HasReserve = HasMember<InspectReserve, Tp>;

  template <typename Tp>
  using IsCopyAssignable = HasMember<InspectCopyAssignable, Tp>;

  template <typename Tp>
  using ValueType = typename Tp::value_type;

  template <typename Tp>
  using VariadicType = typename std::remove_reference_t<Tp>::variadic;

  template <typename Tp>
  using HasValueType = HasMember<ValueType, Tp>;

  template <typename Tp>
  using HasVariadicType = HasMember<VariadicType, Tp>;
}
}

#endif
