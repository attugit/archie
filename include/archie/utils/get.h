#ifndef ARCHIE_UTILS_GET_H_INCLUDED
#define ARCHIE_UTILS_GET_H_INCLUDED

#include <archie/utils/unique_tuple.h>

namespace archie {
namespace utils {

  template <typename Tp, typename... Types>
  const Tp& get(UniqueTuple<Types...> const& ut) {
    return std::get<Tp>(ut);
  }

  template <typename Tp, typename... Types>
  Tp& get(UniqueTuple<Types...>& ut) {
    return std::get<Tp>(ut);
  }

  template <typename Tp, typename... Types>
  const Tp& get(UniqueTuple<Types&...> const& ut) {
    return std::get<Tp&>(ut);
  }

  template <typename Tp, typename... Types>
  Tp& get(UniqueTuple<Types&...>& ut) {
    return std::get<Tp&>(ut);
  }
}
}

#endif
