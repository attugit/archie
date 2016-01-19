#pragma once
#include <utility>
#include <type_traits>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace detail {
  template <typename...>
  struct to_function_pointer_;

  template <typename R, typename... Args>
  struct to_function_pointer_<R(Args...)> {
    using pointer = R (*)(Args...);

    template <typename T>
    std::enable_if_t<std::is_convertible<T, pointer>::value, pointer> operator()(T t) const {
      return t;
    }

    template <typename T>
    std::enable_if_t<!std::is_convertible<T, pointer>::value && std::is_empty<T>::value &&
                         std::is_trivially_constructible<T>::value,
                     pointer>
    operator()(T) const {
      return this->operator()(
          [](Args... args) { return std::add_const_t<T>{}(std::forward<Args>(args)...); });
    }
  };
}

template <typename...>
struct pure_function;

template <typename R, typename... Args>
struct pure_function<R(Args...)> {
  using type = R (*)(Args...);
  using pointer = type;

  pure_function() = default;

  template <typename T>
  explicit pure_function(T t)
      : fptr(meta::instance<detail::to_function_pointer_<R(Args...)>>()(t)) {}

  template <typename T>
  pure_function& operator=(T t) {
    fptr = meta::instance<detail::to_function_pointer_<R(Args...)>>()(t);
    return *this;
  }

  template <typename... U>
  auto operator()(U&&... u) const {
    return (*fptr)(std::forward<U>(u)...);
  }

  explicit operator bool() const { return fptr != nullptr; }
  operator pointer() const { return fptr; }

private:
  pointer fptr = nullptr;
};
}
