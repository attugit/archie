#pragma once

#include <tuple>
namespace archie::fused
{
  using std::tuple;
  using std::get;
  using std::tuple_element;
  namespace detail
  {
    struct make_tuple_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const
      {
        return std::make_tuple(std::forward<Ts>(ts)...);
      }
    };
    struct tie_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const
      {
        return std::tie(std::forward<Ts>(ts)...);
      }
    };
  }
}

#include <archie/fused/type_tag.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct tuple_size_ {
      template <typename Tp>
      constexpr decltype(auto) operator()(type_tag<Tp>) const
      {
        return impl(fused::id<std::decay_t<Tp>>);
      }
      template <typename Tp>
      constexpr decltype(auto) operator()(Tp const&) const
      {
        return impl(fused::id<Tp>);
      }

    private:
      template <typename... Ts>
      constexpr decltype(auto) impl(type_tag<fused::tuple<Ts...>>) const
      {
        return sizeof...(Ts);
      }
    };

    template <std::size_t n>
    struct at_ {
      template <typename Up>
      constexpr decltype(auto) operator()(Up&& up) const
      {
        return fused::get<n>(std::forward<Up>(up));
      }
    };

    template <typename Tp>
    struct extract_ {
      template <typename Up>
      constexpr decltype(auto) operator()(Up&& up) const
      {
        return fused::get<Tp>(std::forward<Up>(up));
      }
    };
  }

  template <std::size_t I, typename Tp>
  using tuple_element_t = meta::eval<fused::tuple_element<I, Tp>>;

  static constexpr auto const& make_tuple = meta::instance<detail::make_tuple_>();
  static constexpr auto const& tie = meta::instance<detail::tie_>();
  static constexpr auto const& tuple_size = meta::instance<detail::tuple_size_>();

  template <std::size_t N>
  static constexpr auto const& at = meta::instance<detail::at_<N>>();
  template <typename T>
  static constexpr auto const& extract = meta::instance<detail::extract_<T>>();

  constexpr auto const apply_to = [](auto&& f) {
    return [f = std::forward<decltype(f)>(f)](auto&& t)->decltype(auto)
    {
      return std::apply(std::forward<decltype(f)>(f), std::forward<decltype(t)>(t));
    };
  };
}
