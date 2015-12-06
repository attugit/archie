#pragma once
#include <utility>
#include <archie/traits/model_of.h>

namespace archie {

struct null_inapt_t {};

template <typename T, typename P>
struct inapt_t {
  using value_type = T;
  using type = value_type;
  using pointer = T*;
  using const_pointer = T const*;
  using reference = T&;
  using const_reference = T const&;

private:
  struct is_valid_policy {
    template <typename Policy>
    auto requires(Policy) -> decltype(std::declval<Policy>().null(),
                                      std::declval<Policy>().is_null(std::declval<T>()));
  };
  static_assert(traits::model_of<is_valid_policy(P)>::value == true,
                "Inapt policy must provide null() and is_null(T) methods");
  struct impl_ : P {
    impl_() : P(), value(P::null()) {}
    bool is_null() const { return P::is_null(value); }
    template <typename... U>
    impl_(U&&... u)
        : value(std::forward<U>(u)...) {}
    void set_null() { value = P::null(); }
    value_type value;
  };
  impl_ impl;

public:
  inapt_t() = default;
  inapt_t(inapt_t const&) = default;
  inapt_t(inapt_t&&) = default;
  inapt_t& operator=(inapt_t const&) = default;
  inapt_t& operator=(inapt_t&&) = default;

  template <typename... U>
  explicit inapt_t(U&&... u)
      : impl(std::forward<U>(u)...) {}

  template <typename U>
  inapt_t& operator=(U const& u) {
    impl.value = u;
    return *this;
  }

  inapt_t& operator=(null_inapt_t const&) {
    impl.set_null();
    return *this;
  }

  explicit operator bool() const { return !is_null(); }
  explicit operator const_reference() const { return get(); }

  bool is_null() const { return impl.is_null(); }
  const_reference get() const { return impl.value; }
  reference get() { return impl.value; }

  bool operator==(null_inapt_t const&) const { return is_null(); }
  bool operator!=(null_inapt_t const&) const { return !is_null(); }
  template <typename U>
  bool operator==(U const& rhs) const {
    return get() == rhs;
  }
  template <typename U>
  bool operator!=(U const& rhs) const {
    return !(get() == rhs);
  }

  reference operator*() { return get(); }
  const_reference operator*() const { return get(); }
  pointer operator->() { return &get(); }
  const_pointer operator->() const { return &get(); }
};

namespace detail {
  template <typename T, T...>
  struct reserved_value_t;

  template <typename T, T v0, T... vs>
  struct reserved_value_t<T, v0, vs...> {
    T null() const { return v0; }
    bool is_null(T const& x) const { return (x == v0) || reserved_value_t<T, vs...>{}.is_null(x); }
  };

  template <typename T, T v0>
  struct reserved_value_t<T, v0> {
    T null() const { return v0; }
    bool is_null(T const& x) const { return x == v0; }
  };
}

template <typename T, T... v>
using reserved_t = inapt_t<T, detail::reserved_value_t<T, v...>>;

template <typename T>
using reserved_default_t = reserved_t<T, T{}>;
}
