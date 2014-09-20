#ifndef ARCHIE_UTILS_SELECT_H_INCLUDED
#define ARCHIE_UTILS_SELECT_H_INCLUDED

#include <type_traits>
#include <archie/utils/typeholder.h>
#include <archie/utils/requires.h>
#include <archie/utils/typeset.h>

namespace archie {
namespace utils {

template <typename...>
struct is_TypeHolder;

template <typename T, typename U>
struct is_TypeHolder<T, U> : public std::is_base_of<TypeHolder<U>, T> {};

template <typename T>
struct is_TypeHolder<T> : public is_TypeHolder<T, typename T::value_type> {};

template <typename T, typename Set, Requires<is_TypeHolder<T>>...>
typename T::reference select(Set& set) {
  return *(get<T>(set));
}

template <typename T, typename Set, Requires<is_TypeHolder<T>>...>
typename T::const_reference select(Set const& set) {
  return *(get<T>(set));
}
}
}

#endif
