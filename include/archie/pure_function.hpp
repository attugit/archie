#pragma once
#include <utility>
#include <type_traits>

namespace archie
{
  template <typename...>
  struct pure_function;

  template <typename R, typename... Args>
  struct pure_function<R(Args...)> {
    using type = R (*)(Args...);

    pure_function() = default;

    template <typename T>
    explicit pure_function(T t) : fptr(nullptr)
    {
      if constexpr(std::is_convertible<T, type>::value) {
        fptr = t;
      } else {
        (void)t;
        fptr = [](auto... args) {
          return std::remove_reference_t<T>{}(std::forward<decltype(args)>(args)...);
        };
      }
    }

    template <typename T>
    pure_function& operator=(T t)
    {
      *this = pure_function(t);
      return *this;
    }

    template <typename... U>
    decltype(auto) operator()(U&&... u) const
    {
      return (*fptr)(std::forward<U>(u)...);
    }

    explicit operator bool() const { return fptr != nullptr; }
    operator type() const { return fptr; }
  private:
    type fptr = nullptr;
  };
}
