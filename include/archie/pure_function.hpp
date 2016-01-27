#pragma once
#include <utility>
#include <type_traits>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/fused/if_t.hpp>

namespace archie {
namespace detail {
  template <typename...>
  struct to_function_pointer_;

  template <typename R, typename... Args>
  struct to_function_pointer_<R(Args...)> {
    using type = R (*)(Args...);

    template <typename T>
    type operator()(T&& t) const {
      return fused::if_(std::is_convertible<std::decay_t<T>, type>{},
                        [](auto&& x) -> type { return std::forward<decltype(x)>(x); },
                        [](auto&& x) -> type {
                          using ObjT = std::decay_t<decltype(x)>;
                          static_assert(std::is_empty<ObjT>::value &&
                                            std::is_trivially_constructible<ObjT>::value,
                                        "");
                          return [](Args... args) {
                            return std::add_const_t<ObjT>{}(std::forward<Args>(args)...);
                          };
                        })(std::forward<T>(t));
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
