#ifndef ARCHIE_UTILS_TYPE_SET_H_INCLUDED
#define ARCHIE_UTILS_TYPE_SET_H_INCLUDED

#include <utility>
#include <archie/utils/requires.h>
#include <type_traits>

namespace archie {
namespace utils {

template <typename Head, typename... Tail>
struct TypeSet : TypeSet<Head>, TypeSet<Tail...> {
  TypeSet(Head head, Tail... tail)
      : TypeSet<Head>(std::forward<Head>(head)),
        TypeSet<Tail...>(std::forward<Tail>(tail)...) {}

  template <typename T, Requires<std::is_same<T, Head>>...>
  const T& get() const {
    return TypeSet<Head>::template get<T>();
  }
  template <typename T, Requires<std::is_same<T, Head>>...>
  T& get() {
    return TypeSet<Head>::template get<T>();
  }
  template <typename T, Requires<Not<std::is_same<T, Head>>>...>
  T const& get() const {
    return TypeSet<Tail...>::template get<T>();
  }
  template <typename T, Requires<Not<std::is_same<T, Head>>>...>
  T& get() {
    return TypeSet<Tail...>::template get<T>();
  }

  constexpr std::size_t size() const { return 1 + sizeof...(Tail); }
};

template <typename Head>
struct TypeSet<Head> {
  template <typename U>
  explicit TypeSet(U u)
      : value(std::forward<U>(u)) {}

  template <typename T>
  T const& get() const {
    static_assert(std::is_same<T, Head>::value, "Type not found in TypeSet");
    return value;
  }
  template <typename T>
  T& get() {
    static_assert(std::is_same<T, Head>::value, "Type not found in TypeSet");
    return value;
  }

  constexpr std::size_t size() const { return 1; }

private:
  Head value;
};

template <typename T, typename... Types>
const T& get(TypeSet<Types...> const& ts) {
  return ts.get<T>();
}

template <typename T, typename... Types>
T& get(TypeSet<Types...>& ts) {
  return ts.get<T>();
}

template <typename... Types>
TypeSet<Types...> make_type_set(Types&&... args) {
  return TypeSet<Types...>(std::forward<Types>(args)...);
}

template <typename... Types>
constexpr std::size_t type_set_size(TypeSet<Types...> const&) {
  return sizeof...(Types);
}
}
}

#endif
