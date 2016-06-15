#pragma once

#include <utility>
#include <archie/meta.hpp>
#include <archie/ignore.hpp>
#include <archie/meta/indexable.hpp>

namespace archie::meta
{
  namespace detail
  {
    template <std::size_t n, typename... Ts>
    struct at_ {
    private:
      template <typename... other>
      struct impl_ {
        template <typename Tp>
        static auto skip(eat<other>..., Tp&&, ...) noexcept -> returns<Tp>;

        template <typename... Us>
        using apply = decltype(skip(std::declval<Us>()...));
      };

    public:
      using type = typename indexable_t<impl_, n>::template apply<Ts...>;
    };
  }

  template <std::size_t n, typename... Ts>
  struct at : detail::at_<n, Ts...>::type {
  };

  template <std::size_t n, typename... Ts>
  struct at<n, type_list<Ts...>> : at<n, Ts...> {
  };

  template <std::size_t n, typename... Ts>
  using at_t = eval<at<n, Ts...>>;
}
