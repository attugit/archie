#pragma once

#include <utility>

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
}

namespace archie::meta
{
  template <typename Tp>
  using eval = typename Tp::type;
}

namespace archie::meta
{
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
}

namespace archie::meta
{
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
}

namespace archie::meta
{
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
}

namespace archie::meta
{
  template <std::size_t N>
  struct number : std::integral_constant<std::size_t, N> {
  };
}

namespace archie::fused
{
  template <std::size_t N>
  static constexpr auto const& number = meta::instance<meta::number<N>>();
}

namespace archie::meta
{
  template <typename T>
  struct quote : comparable<quote<T>> {
  };
}

namespace archie::fused
{
  template <typename T>
  static constexpr auto const& quote = meta::instance<meta::quote<T>>();
}

namespace archie::meta
{
  template <typename Tp>
  struct identity {
    using type = Tp;
  };

  template <typename Tp>
  using identity_t = eval<identity<Tp>>;
}

namespace archie::meta
{
  template <typename Tp>
  using returns = identity<Tp>;
}
