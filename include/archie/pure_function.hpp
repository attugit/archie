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
    explicit pure_function(T t) : fptr(convert(t))
    {
    }

    template <typename T>
    pure_function& operator=(T t)
    {
      fptr = convert(t);
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
    template <typename T>
    static constexpr type convert(T&& t)
    {
      if
        constexpr(std::is_convertible<std::decay_t<T>, type>::value) { return std::forward<T>(t); }
      else
      {
        return [](Args... args) -> R {
          return std::remove_reference_t<T const>{}(std::forward<Args>(args)...);
        };
      }
    }

    type fptr = nullptr;
  };
}
