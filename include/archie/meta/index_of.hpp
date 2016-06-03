#pragma once

#include <utility>
#include <archie/meta/ignore.hpp>
#include <archie/meta/number.hpp>
#include <archie/type_list.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::meta
{
  namespace detail
  {
    template <typename T>
    struct index_of_ {
    private:
      template <std::size_t N>
      struct state_ {
        constexpr operator std::size_t() const noexcept { return N; }
        template <typename U>
        constexpr auto operator&(U&&)const noexcept
        {
          return *this;
        }
      };
      template <std::size_t N>
      struct accumulator_ : state_<N> {
        template <typename U>
        constexpr auto operator&(U) const noexcept
        {
          using next = std::conditional_t<U::value, state_<N>, accumulator_<N + 1>>;
          return next{};
        }
      };
      template <typename... U>
      constexpr auto impl(U... u) const noexcept
      {
        return meta::number<(... & u)>{};
      }

    public:
      template <typename... U>
      constexpr auto operator()(type_list<U...>) const noexcept
      {
        return impl(accumulator_<0>{}, std::is_same<T, U>{}...);
      }
    };
  }

  template <typename T>
  static constexpr auto const& index_of = meta::instance<detail::index_of_<T>>();

  template <typename Tp, typename... Ts>
  constexpr decltype(auto) type_index(Ts...) noexcept
  {
    return index_of<Tp>(type_list<Ts...>{});
  }
}
