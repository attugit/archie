#pragma once

#include <archie/meta.hpp>
#include <archie/type_list.hpp>
#include <archie/search_for.hpp>

namespace archie::fused
{
  namespace detail
  {
    template <typename T>
    struct index_of_ {
    private:
      struct pred_ {
        template <typename U>
        constexpr auto operator()(meta::quote<U> u) const noexcept
        {
          return fused::quote<T> == u;
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
