#ifndef ARCHIE_UTILS_SELECT_H_INCLUDED
#define ARCHIE_UTILS_SELECT_H_INCLUDED

#include <type_traits>
#include <archie/utils/typeholder.h>
#include <archie/utils/requires.h>
#include <tuple>

namespace archie {
namespace utils {

  template <typename...>
  struct IsTypeHolder;

  template <typename T, typename U>
  struct IsTypeHolder<T, U> : public std::is_base_of<TypeHolder<U>, T> {};

  template <typename T>
  struct IsTypeHolder<T> : public IsTypeHolder<T, typename T::value_type> {};

  template <typename T, typename Set, typename = Requires<IsTypeHolder<T>>>
  auto select(Set& set) -> decltype(*(std::declval<T>())) {
    return *(std::get<T>(set));
  }

  template <typename T, typename Set, typename = Requires<IsTypeHolder<T>>>
  auto select(Set const& set) -> decltype(*(std::declval<T>())) {
    return *(std::get<T>(set));
  }
}
}

#endif
