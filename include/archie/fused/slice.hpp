#pragma once

#include <utility>
#include <archie/meta.hpp>
#include <archie/fused/apply.hpp>
#include <archie/fused/take.hpp>

namespace archie::fused
{
  namespace detail
  {
    template <std::size_t S>
    struct slice_ {
      template <typename... ids>
      struct block_ {
        template <typename T>
        constexpr decltype(auto) operator()(T&& t) const
        {
          return make_tuple(fused::get<S + ids::value>(std::forward<T>(t))...);
        }
      };
      template <std::size_t E>
      using type = meta::indexable_t<block_, E - S>;
    };
  }

  template <std::size_t S, std::size_t E>
  static constexpr auto const& slice =
      meta::instance<typename detail::slice_<S>::template type<E>>();
}
