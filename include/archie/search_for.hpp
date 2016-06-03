#pragma once

#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/number.hpp>

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
      constexpr auto operator()(F f, U... u) const noexcept
      {
        (void)f; // unused when passing empty parameter pack
        return impl(accumulator_<0>{}, f(u)...);
      }
    };
  }

  static constexpr auto const& search_for = meta::instance<detail::search_for_>();
}
