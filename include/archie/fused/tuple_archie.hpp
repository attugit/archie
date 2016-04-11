#pragma once

#if defined(USE_ARCHIE_TUPLE)
#include <utility>
#include <array>
#include <archie/meta/eval.hpp>
#include <archie/meta/index_of.hpp>
#include <archie/meta/requires.hpp>
#include <archie/meta/at.hpp>
#include <archie/meta/indexable.hpp>
#include <archie/meta/logic.hpp>
#include <archie/traits.hpp>
#include <archie/fused/nth.hpp>
#include <archie/fused/mover.hpp>
#include <archie/fused/ignore.hpp>
#include <archie/traits/is_callable.hpp>
#include <archie/fused/static_if.hpp>
#include <archie/fused/boolean.hpp>

namespace archie {
namespace fused {
  template <typename... Ts>
  struct tuple {
  private:
    template <typename... Us>
    static decltype(auto) make_storage(Us... args) {
      return [args...](auto&& func) mutable -> decltype(auto) {
        return std::forward<decltype(func)>(func)(args...);
      };
    }
    using storage_type = decltype(make_storage(std::declval<move_t<Ts>>()...));

    storage_type storage;

    struct explicit_ctor_tag {};
    struct implicit_ctor_tag {};
    struct move_t_ctor_tag {};

    template <typename... Us>
    constexpr explicit tuple(implicit_ctor_tag, Us&&... args)
        : tuple(move_t_ctor_tag{}, move_t<Ts>(std::forward<Us>(args))...) {}

    constexpr explicit tuple(move_t_ctor_tag, move_t<Ts>... mvs) : storage(make_storage(mvs...)) {}

    template <typename... Us>
    constexpr explicit tuple(explicit_ctor_tag, Us&&... args)
        : tuple(Ts{std::forward<Us>(args)}...) {}

    template <typename F, typename... Us>
    using requires_callable = meta::requires<traits::is_callable<F, Us...>>;

  public:
    template <typename... Us>
    constexpr explicit tuple(Us&&... args)
        : tuple(meta::if_t<meta::all<traits::is_convertible<Us, Ts>...>,
                           implicit_ctor_tag,
                           explicit_ctor_tag>{},
                std::forward<Us>(args)...) {}

    constexpr tuple() : tuple(Ts{}...) {}
    tuple(tuple& other) : tuple(const_cast<tuple const&>(other)) {}

    tuple(tuple&&) = default;
    tuple(tuple const&) = default;
    tuple& operator=(tuple const&);
    tuple& operator=(tuple&&);

    template <typename... Us>
    tuple& operator=(tuple<Us...> const&);

    template <typename... Us>
    bool operator==(tuple<Us...> const&) const;
    bool operator<(tuple const&) const;

    bool operator!=(tuple const& rhs) const { return !(*this == rhs); }
    bool operator>(tuple const& rhs) const { return rhs < *this; }
    bool operator>=(tuple const& rhs) const { return !(*this < rhs); }
    bool operator<=(tuple const& rhs) const { return !(*this > rhs); }

    constexpr std::size_t size() noexcept { return sizeof...(Ts); }

    template <typename F>
    decltype(auto) apply(F&& f, requires_callable<F, Ts const&...> = fused::ignore) const& {
      auto exec = [&f](move_t<Ts>&... xs) -> decltype(auto) {
        return std::forward<decltype(f)>(f)(static_cast<Ts const&>(xs)...);
      };
      // TODO explain why const_cast is used here
      return const_cast<storage_type&>(storage)(exec);
    }

    template <typename F>
    decltype(auto) apply(F&& f, requires_callable<F, Ts&...> = fused::ignore) & {
      auto exec = [&f](move_t<Ts>&... xs) -> decltype(auto) {
        return std::forward<decltype(f)>(f)(static_cast<std::add_lvalue_reference_t<Ts>>(xs)...);
      };
      return storage(exec);
    }

    template <typename F>
    decltype(auto) apply(F&& f, requires_callable<F, Ts&&...> = fused::ignore) && {
      auto exec = [&f](move_t<Ts>&... xs) -> decltype(auto) {
        return std::forward<decltype(f)>(f)(static_cast<Ts&&>(xs)...);
      };
      return storage(exec);
    }
  };

  namespace detail {
    struct make_tuple_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... args) const {
        return fused::tuple<std::remove_reference_t<Ts>...>(std::forward<Ts>(args)...);
      }
    };

    struct tie_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&... args) const {
        return fused::tuple<Ts&...>(args...);
      }
    };
  }

  template <std::size_t N, typename Tp>
  decltype(auto) get(Tp&& tp) {
    return tp.apply(fused::nth<N>);
  }

  template <typename Tp, typename... Us>
  decltype(auto) get(tuple<Us...>& u) {
    constexpr auto idx = meta::index_of<Tp>(meta::type_list<Us...>{});
    return get<idx>(u);
  }

  template <typename Tp, typename... Us>
  decltype(auto) get(tuple<Us...> const& u) {
    constexpr auto idx = meta::index_of<Tp>(meta::type_list<Us...>{});
    return get<idx>(u);
  }

  template <std::size_t I, typename Tp>
  struct tuple_element;

  template <std::size_t I, typename... Ts>
  struct tuple_element<I, tuple<Ts...>> {
    using type = meta::at_t<I, Ts...>;
  };

  namespace detail {
    template <std::size_t head, std::size_t... tail>
    struct tuple_recursion {
      template <typename F, typename L, typename R>
      static bool call(F f, L const& x, R const& y) {
        return fused::static_if(meta::boolean<sizeof...(tail)>{})(
            [f](auto const& x1, auto const& y1) -> bool {
              return get<head>(x1) == get<head>(y1) ? tuple_recursion<tail...>::call(f, x1, y1)
                                                    : tuple_recursion<head>::call(f, x1, y1);
            },
            [f](auto const& x2, auto const& y2) -> bool {
              return f(get<head>(x2), get<head>(y2));
            })(x, y);
      }
      template <typename... Ts, typename... Us>
      static bool equal(tuple<Ts...> const& x, tuple<Us...> const& y) {
        return call([](auto const& lhs, auto const& rhs) { return lhs == rhs; }, x, y);
      }
      template <typename... Ts>
      static bool less(tuple<Ts...> const& x, tuple<Ts...> const& y) {
        return call([](auto const& lhs, auto const& rhs) { return lhs < rhs; }, x, y);
      }
    };

    template <std::size_t... idx>
    struct tuple_internals {
      template <typename... Ts, typename... Us>
      static void copy_assign(tuple<Ts...>& lhs, tuple<Us...> const& rhs) {
        meta::ignore{(fused::get<idx>(lhs) = fused::get<idx>(rhs), false)...};
      }
      template <typename... Ts, typename... Us>
      static void move_assign(tuple<Ts...>& lhs, tuple<Us...>&& rhs) {
        meta::ignore{(fused::get<idx>(lhs) = std::move(fused::get<idx>(rhs)), false)...};
      }
      template <typename... Ts, typename... Us>
      static bool equal(tuple<Ts...> const& lhs, tuple<Us...> const& rhs) {
        return tuple_recursion<idx...>::equal(lhs, rhs);
      }
      template <typename... Ts>
      static bool less(tuple<Ts...> const& lhs, tuple<Ts...> const& rhs) {
        return tuple_recursion<idx...>::less(lhs, rhs);
      }
    };
  }

  template <typename... Ts>
  tuple<Ts...>& tuple<Ts...>::operator=(tuple<Ts...> const& orig) {
    meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::copy_assign(*this, orig);
    return *this;
  }

  template <typename... Ts>
  tuple<Ts...>& tuple<Ts...>::operator=(tuple<Ts...>&& orig) {
    meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::move_assign(*this, std::move(orig));
    return *this;
  }

  template <typename... Ts>
  template <typename... Us>
  tuple<Ts...>& tuple<Ts...>::operator=(tuple<Us...> const& orig) {
    static_assert(sizeof...(Ts) <= sizeof...(Us), "");
    meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::copy_assign(*this, orig);
    return *this;
  }

  template <typename... Ts>
  template <typename... Us>
  bool tuple<Ts...>::operator==(tuple<Us...> const& rhs) const {
    return meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::equal(*this, rhs);
  }

  template <typename... Ts>
  bool tuple<Ts...>::operator<(tuple<Ts...> const& rhs) const {
    return meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::less(*this, rhs);
  }
}
}
#endif
