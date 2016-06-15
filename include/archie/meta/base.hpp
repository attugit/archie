#pragma once

#include <utility>
#include <type_traits>

namespace archie::meta
{
  template <typename T>
  struct static_constexpr_storage {
    static constexpr T const value{};
  };

  template <typename T>
  constexpr T const static_constexpr_storage<T>::value;

  template <typename T>
  constexpr auto const& instance() noexcept
  {
    return static_constexpr_storage<T>::value;
  }

  template <typename Tp>
  using eval = typename Tp::type;

  template <bool B>
  struct boolean : std::integral_constant<bool, B> {
  };

  using true_t = boolean<true>;
  using false_t = boolean<false>;
}

namespace archie::fused
{
  template <bool B>
  static constexpr auto const& boolean = meta::instance<meta::boolean<B>>();
  static constexpr auto const True = boolean<true>;
  static constexpr auto const False = boolean<false>;
}

namespace archie::meta
{
  template <bool B1, bool B2>
  constexpr decltype(auto) operator==(boolean<B1> const&, boolean<B2> const&) noexcept
  {
    return fused::boolean<B1 == B2>;
  }

  template <bool B1, bool B2>
  constexpr decltype(auto) operator==(boolean<B1> const&,
                                      std::integral_constant<bool, B2> const&) noexcept
  {
    return fused::boolean<B1 == B2>;
  }

  template <bool B1, bool B2>
  constexpr decltype(auto) operator!=(boolean<B1> const&, boolean<B2> const&) noexcept
  {
    return fused::boolean<B1 != B2>;
  }

  template <bool B1, bool B2>
  constexpr decltype(auto) operator!=(boolean<B1> const&,
                                      std::integral_constant<bool, B2> const&) noexcept
  {
    return fused::boolean<B1 != B2>;
  }

  template <typename>
  struct comparable {
  };

  template <typename T>
  constexpr auto operator==(comparable<T> const&, comparable<T> const&) noexcept
  {
    return fused::True;
  }

  template <typename T, typename U>
  constexpr auto operator==(comparable<T> const&, comparable<U> const&) noexcept
  {
    return fused::False;
  }

  template <typename T>
  constexpr auto operator!=(comparable<T> const&, comparable<T> const&) noexcept
  {
    return fused::False;
  }

  template <typename T, typename U>
  constexpr bool operator!=(comparable<T> const&, comparable<U> const&) noexcept
  {
    return fused::True;
  }

  template <typename... Ts>
  struct all : boolean<(... && Ts::value)> {
  };

  template <typename... Ts>
  struct any : boolean<(... || Ts::value)> {
  };

  template <typename... Ts>
  using none = boolean<!any<Ts...>::value>;

  template <typename T>
  using no = none<T>;

  template <std::size_t N>
  struct number : std::integral_constant<std::size_t, N> {
  };

  template <typename T>
  struct quote : comparable<quote<T>> {
    using type = T;
  };

  template <typename T>
  using identity = quote<T>;

  template <typename T>
  using identity_t = eval<identity<T>>;

  template <typename Tp>
  using returns = identity<Tp>;

  template <typename... T>
  struct type_list : comparable<type_list<T...>> {
  };

  template <typename Condition, typename T, typename U>
  using if_t = std::conditional_t<Condition::value, T, U>;

  template <std::size_t... N>
  struct sum : number<(N + ...)> {
  };
}

namespace archie::fused
{
  template <std::size_t N>
  static constexpr auto const& number = meta::instance<meta::number<N>>();

  template <typename T>
  static constexpr auto const& quote = meta::instance<meta::quote<T>>();

  template <typename... T>
  static constexpr auto const& type_list = meta::instance<meta::type_list<T...>>();
}
