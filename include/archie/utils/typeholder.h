#ifndef ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED

#include <archie/utils/operators.h>

namespace archie {
namespace utils {

  template <typename T>
  struct TypeHolder : Operators<TypeHolder<T>> {
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    template <typename U>
    explicit TypeHolder(TypeHolder<U> other)
        : value(*other) {}
    template <typename U>
    explicit TypeHolder(U u)
        : value(u) {}
    template <typename U>
    U& get() {
      return value;
    }
    template <typename U>
    U const& get() const {
      return value;
    }
    reference operator*() { return get<value_type>(); }
    const_reference operator*() const { return get<value_type>(); }

    friend bool operator<(TypeHolder const& lhs, TypeHolder const& rhs) {
      return (&lhs != &rhs) ? lhs.value < rhs.value : false;
    }

  private:
    value_type value;
  };
}
}

#define ARCHIE_UTILS_GENERATE_TYPE_HOLDER(name, underlying)                    \
  struct name : archie::utils::TypeHolder<underlying> {                        \
    using BaseType = archie::utils::TypeHolder<underlying>;                    \
    using value_type = typename BaseType::value_type;                          \
    using reference = typename BaseType::reference;                            \
    using const_reference = typename BaseType::const_reference;                \
    template <typename U>                                                      \
    explicit name(U u)                                                         \
        : BaseType(u) {}                                                       \
    using BaseType::get;                                                       \
    using BaseType::operator*;                                                 \
  }

#endif
