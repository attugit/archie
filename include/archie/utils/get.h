#ifndef ARCHIE_UTILS_GET_H_INCLUDED
#define ARCHIE_UTILS_GET_H_INCLUDED

#include <archie/utils/unique_tuple.h>
#include <functional>

namespace archie {
namespace utils {

  template <typename Tp>
  struct UnwrapReference {
    using type = Tp;
  };

  template <typename Tp>
  struct UnwrapReference<Tp&> {
    using type = Tp;
  };

  template <typename Tp>
  struct UnwrapReference<Tp&&> {
    using type = Tp;
  };

  template <typename Tp>
  struct UnwrapReference<std::reference_wrapper<Tp>> {
    using type = Tp;
  };

  template <typename Tp>
  using UnwrapReferenceType = typename UnwrapReference<Tp>::type;

  template <typename Tp, typename... Types>
  UnwrapReferenceType<Tp> const& get(UniqueTuple<Types...> const& ut) {
    return std::get<UnwrapReferenceType<Tp>>(ut);
  }

  template <typename Tp, typename... Types>
  UnwrapReferenceType<Tp>& get(UniqueTuple<Types...>& ut) {
    return std::get<UnwrapReferenceType<Tp>>(ut);
  }

  template <typename Tp, typename... Types>
  Tp const& get(UniqueTuple<std::reference_wrapper<Types>...> const& ut) {
    return std::get<std::reference_wrapper<Tp>>(ut);
  }

  template <typename Tp, typename... Types>
  Tp& get(UniqueTuple<std::reference_wrapper<Types>...>& ut) {
    return std::get<std::reference_wrapper<Tp>>(ut);
  }
}
}

#endif
