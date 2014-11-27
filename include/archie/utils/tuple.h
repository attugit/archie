#ifndef ARCHIE_UTILS_TUPLE_H_INCLUDED
#define ARCHIE_UTILS_TUPLE_H_INCLUDED

#include <utility>
#include <archie/utils/alias.h>
#include <archie/utils/number.h>
#include <archie/utils/variadic.h>
#include <archie/utils/eat.h>

namespace archie {
namespace utils {

  template <typename... Ts>
  struct Tuple {
  private:
    template <typename... Args>
    constexpr static decltype(auto) make_storage(Args&&... args) {
      return [args...](auto&& func) -> decltype(auto) {
        return std::forward<decltype(func)>(func)(args...);
      };
    }
    using Storage = decltype(make_storage(std::declval<Ts>()...));
    Storage storage;

  public:
    using variadic = Variadic<Ts...>;

    template <typename... Up>
    constexpr explicit Tuple(Up&&... u)
        : storage(make_storage(std::forward<Up>(u)...)) {}

    constexpr Tuple() : Tuple(Ts {}...) {}

    constexpr decltype(auto) size() const { return variadic::size(); }

    template <typename Func>
    decltype(auto) apply(Func&& func) {
      return storage(std::forward<Func>(func));
    }

    template <typename Func>
    decltype(auto) for_each(Func&& func) {
      auto functor = [f = std::forward<Func>(func)](auto&&... xs) {
        (void)Alias<int[]>{
            (std::forward<decltype(f)>(f)(std::forward<decltype(xs)>(xs)),
             0)...};
        return std::move(f);
      };
      return storage(functor);
    }
  };

  namespace detail {
    struct at_impl {
      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct get;

      template <std::size_t n, std::size_t... ignore>
      struct get<n, std::index_sequence<ignore...>> {
        template <typename Nth, typename...>
        constexpr decltype(auto) operator()(eat<meta::number<ignore>>...,
                                            Nth&& nth, ...) const {
          return std::forward<Nth>(nth);
        }
      };
    };
    template <typename... Ts>
    struct skip {
      template <typename Up>
      static constexpr decltype(auto) get(eat<Ts>..., Up&& u, ...) noexcept {
        return std::forward<Up>(u);
      }
    };
  }

  template <typename Tp, typename... Ts>
  constexpr decltype(auto) back(Tp&& t, Ts&&... ts) noexcept {
    return detail::skip<Ts...>::get(std::forward<Tp>(t),
                                    std::forward<Ts>(ts)...);
  }

  template <std::size_t n, typename TupleType>
  constexpr decltype(auto) get(TupleType&& t) noexcept {
    return std::forward<TupleType>(t).apply(detail::at_impl::get<n>{});
  }

  template <typename Tp, typename TupleType>
  constexpr decltype(auto) get(TupleType&& tuple) noexcept {
    using index =
        meta::number<VariadicType<TupleType>::template index_of<Tp>()>;
    return get<index::value>(std::forward<TupleType>(tuple));
  }
}
}

#endif
