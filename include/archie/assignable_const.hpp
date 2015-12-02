#pragma once
#include <type_traits>
#include <utility>

namespace archie {
template <typename T>
struct assignable_const {
  static_assert(!std::is_const<T>::value, "");
  using value_type = T;
  using const_reference = T const&;

  template <typename... Args>
  explicit assignable_const(Args&&... args)
      : data_(std::forward<Args>(args)...) {}
  assignable_const(assignable_const const&) = default;
  assignable_const(assignable_const&&) = default;
  assignable_const& operator=(assignable_const const&) = default;
  assignable_const& operator=(assignable_const&&) = default;

  operator const_reference() const { return data_; }

private:
  value_type data_;
};
}
