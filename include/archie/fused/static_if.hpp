#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct static_if_ {
      template <typename Cond>
      decltype(auto) operator()(Cond const&) const
      {
        return select(std::integral_constant<bool, std::decay_t<Cond>::value>{});
      }

    private:
      struct if_true_ {
        template <typename T, typename... U>
        decltype(auto) operator()(T&& t, U&&...) const
        {
          return std::forward<T>(t);
        }
      };
      struct if_false_ {
        template <typename T>
        decltype(auto) operator()(T&&) const
        {
          return [](auto&&...) {};
        }
        template <typename T, typename U>
        decltype(auto) operator()(T&&, U&& u) const
        {
          return std::forward<U>(u);
        }
      };
      decltype(auto) select(std::true_type) const { return if_true_{}; }
      decltype(auto) select(std::false_type) const { return if_false_{}; }
    };
  }

  static constexpr auto const& static_if = meta::instance<detail::static_if_>();
}
