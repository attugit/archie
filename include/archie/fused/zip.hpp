#pragma once

#include <archie/meta/indexable.hpp>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/fused/tuple.hpp>

namespace archie {
namespace fused {
  namespace detail {
    struct zip_impl {
    private:
      template <std::size_t n>
      struct impl_n {
        template <typename F, typename... Ts>
        constexpr decltype(auto) operator()(F&& f, Ts&&... t) const {
          return std::forward<F>(f)(fused::get<n>(std::forward<Ts>(t))...);
        }
      };
      template <std::size_t... ns>
      struct impl {
        template <typename F, typename... Ts>
        constexpr decltype(auto) operator()(F&& f, Ts&&... t) const {
          return fused::make_tuple(impl_n<ns>{}(std::forward<F>(f), std::forward<Ts>(t)...)...);
        }
      };

    public:
      template <typename F, typename Tp, typename... Us>
      constexpr decltype(auto) operator()(F&& f, Tp&& t, Us&&... us) const {
        return meta::indexable_t<impl, tuple_size(type_tag<Tp>{})>{}(
            std::forward<F>(f), std::forward<Tp>(t), std::forward<Us>(us)...);
      }
    };

    struct zip_ : private zip_impl {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const {
        return zip_impl::operator()(fused::make_tuple, std::forward<Ts>(ts)...);
      }
    };

    struct zip_view_ : private zip_impl {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const {
        return zip_impl::operator()(fused::tie, std::forward<Ts>(ts)...);
      }
    };
  }
  static constexpr auto const& zip = meta::instance<detail::zip_>();
  static constexpr auto const& zip_view = meta::instance<detail::zip_view_>();
}
}
