#ifndef ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED

#include <boost/operators.hpp>

namespace archie {
namespace utils {

  template <typename Tp>
  struct TypeHolder
      : boost::totally_ordered<TypeHolder<Tp>,
                               boost::totally_ordered<TypeHolder<Tp>, Tp>> {
    using value_type = Tp;
    using reference = Tp&;
    using const_reference = Tp const&;
    using pointer = Tp*;
    using const_pointer = const Tp*;
    using type = TypeHolder<Tp>;

    template <typename Up>
    TypeHolder(Up&& up)
        : value(std::forward<Up>(up)) {}

    reference operator*() noexcept { return value; }
    const_reference operator*() const noexcept { return value; }
    pointer operator->() noexcept { return &value; }
    const_pointer operator->() const noexcept { return &value; }

    friend bool operator<(type const& lhs, type const& rhs) {
      return lhs.value < rhs.value;
    }
    friend bool operator<(type const& lhs, const_reference rhs) {
      return lhs.value < rhs;
    }
    friend bool operator==(type const& lhs, type const& rhs) {
      return lhs.value == rhs.value;
    }
    friend bool operator==(type const& lhs, const_reference rhs) {
      return lhs.value == rhs;
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
    using BaseType::BaseType;                                                  \
    template <typename U>                                                      \
    name(const U& u)                                                           \
        : BaseType(u) {}                                                       \
    name(name&&) = default;                                                    \
    name(name const&) = default;                                               \
    name& operator=(name&&) = default;                                         \
    name& operator=(name const&) = default;                                    \
    using BaseType::operator*;                                                 \
    using BaseType::operator->;                                                \
  }

#endif
