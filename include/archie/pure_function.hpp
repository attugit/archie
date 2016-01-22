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
    using type = R (*)(Args...);

  private:
    struct convertible_tag {};
    struct stateless_tag {};

    template <typename T>
    type convert(T&& t, convertible_tag) const {
      return std::forward<T>(t);
    }

    template <typename T>
    type convert(T&&, stateless_tag) const {
      using Func = std::decay_t<T>;
      auto const wrap = [](Args... args) {
        return std::add_const_t<Func>{}(std::forward<Args>(args)...);
      };
      return operator()(wrap);
    }

  public:
    template <typename T>
    type operator()(T&& t) const {
      using Func = std::decay_t<T>;
      return convert(
          std::forward<T>(t),
          std::conditional_t<std::is_convertible<Func, type>::value, convertible_tag,
                             std::conditional_t<std::is_empty<Func>::value &&
                                                    std::is_trivially_constructible<Func>::value,
                                                stateless_tag, void>>{});
    }
  };
}

template <typename...>
struct pure_function;

template <typename R, typename... Args>
struct pure_function<R(Args...)> {
  using type = R (*)(Args...);

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
  operator type() const { return fptr; }

private:
  type fptr = nullptr;
};
}
