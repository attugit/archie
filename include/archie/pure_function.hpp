#pragma once
#include <utility>
#include <type_traits>
#include <archie/utils/meta/static_constexpr_storage.h>
#include <archie/utils/traits/is_constructible.h>

namespace archie {
namespace detail {
  template <typename...>
  struct to_function_pointer_;

  template <typename R, typename... Args>
  struct to_function_pointer_<R(Args...)> {
  private:
    template <typename T>
    using trivial = utils::traits::is_trivially_copy_constructible<T>;

  public:
    using pointer = R (*)(Args...);

    template <typename T>
    typename std::enable_if<std::is_convertible<T, pointer>::value, pointer>::type operator()(
        T t) const {
      return t;
    }

    template <typename T>
    typename std::enable_if<!std::is_convertible<T, pointer>::value && std::is_empty<T>::value &&
                                trivial<T>::value,
                            pointer>::type
    operator()(T) const {
      return this->operator()([](Args... args) {
        return typename std::add_const<T>::type{}(std::forward<Args>(args)...);
      });
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
      : fptr(utils::meta::instance<detail::to_function_pointer_<R(Args...)>>()(t)) {}

  template <typename T>
  pure_function& operator=(T t) {
    fptr = utils::meta::instance<detail::to_function_pointer_<R(Args...)>>()(t);
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
