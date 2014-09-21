#ifndef ARCHIE_UTILS_TYPE_SET_H_INCLUDED
#define ARCHIE_UTILS_TYPE_SET_H_INCLUDED

#include <utility>
#include <archie/utils/requires.h>
#include <archie/utils/operators.h>
#include <type_traits>

namespace archie {
namespace utils {

  template <typename... Types>
  struct TypeSet;

  template <typename T, typename... Types>
  T const& get(TypeSet<Types...> const&);

  template <typename Head, typename... Tail>
  struct TypeSet<Head, Tail...>
      : TypeSet<Head>,
        TypeSet<Tail...>,
        Operators<TypeSet<Head, Tail...>, TypeSet<Head, Tail...>> {
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

    using SelfType = TypeSet<Head, Tail...>;

  private:
    template <typename...>
    struct compare;

    template <typename T, typename... U>
    struct compare<T, U...> {
      static bool less(SelfType const& lhs, SelfType const& rhs) {
        return compare<T>::eq(lhs, rhs) ? compare<U...>::less(lhs, rhs)
                                        : compare<T>::less(lhs, rhs);
      };
    };
    template <typename T>
    struct compare<T> {
      static bool less(SelfType const& lhs, SelfType const& rhs) {
        return lhs.get<T>() < rhs.get<T>();
      }
      static bool eq(SelfType const& lhs, SelfType const& rhs) {
        return lhs.get<T>() == rhs.get<T>();
      }
    };

  public:
    friend bool operator<(SelfType const& lhs, SelfType const& rhs) {
      return (&lhs != &rhs) ? compare<Head, Tail...>::less(lhs, rhs) : false;
    }
  };

  template <typename Head>
  struct TypeSet<Head> : Operators<TypeSet<Head>, TypeSet<Head>> {
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

    friend bool operator<(TypeSet<Head> const& lhs, TypeSet<Head> const& rhs) {
      return lhs.get<Head>() < rhs.get<Head>();
    }

  private:
    Head value;
  };

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
