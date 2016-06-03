#pragma once

#include <archie/boolean.hpp>
#include <archie/number.hpp>
#include <archie/type_list.hpp>
#include <archie/quote.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct search_for_ {
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
          return std::conditional_t<U::value, state_<N>, accumulator_<N + 1>>{};
        }
      };
      template <typename... U>
      constexpr auto impl(U... u) const noexcept
      {
        return fused::number<(... & u)>;
      }

    public:
      template <typename F, typename... U>
      constexpr auto operator()(F f, meta::quote<U>... q) const noexcept
      {
        (void)f; // unused when passing empty parameter pack
        return impl(accumulator_<0>{}, f(q)...);
      }
    };
  }

  static constexpr auto const& search_for = meta::instance<detail::search_for_>();

  namespace detail
  {
    template <typename T>
    struct index_of_ {
    private:
      struct pred_ {
        template <typename U>
        constexpr auto operator()(meta::quote<U> u) const noexcept
        {
          return fused::boolean<fused::quote<T> == u>;
        }
      };

    public:
      template <typename... U>
      constexpr auto operator()(meta::type_list<U...>) const noexcept
      {
        return search_for(pred_{}, quote<U>...);
      }
    };
  }

  template <typename T>
  static constexpr auto const& index_of = meta::instance<detail::index_of_<T>>();

  namespace detail
  {
    template <typename T>
    struct type_index_ {
      template <typename... U>
      constexpr auto operator()(U...) const noexcept
      {
        return index_of<T>(type_list<U...>);
      }
    };
  }

  template <typename T>
  static constexpr auto const& type_index = meta::instance<detail::type_index_<T>>();
}
