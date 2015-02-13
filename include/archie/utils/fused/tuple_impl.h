#pragma once

#include <archie/utils/meta/index_of.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/meta/at.h>
#include <archie/utils/meta/indexable.h>
#include <archie/utils/meta/logic.h>
#include <archie/utils/traits.h>
#include <archie/utils/fused/nth.h>
#include <archie/utils/fused/mover.h>
#include <config.h>
#include <utility>
#include <array>

namespace archie {
namespace utils {
  namespace fused {

    template <typename... Ts>
    struct tuple {
    private:
#if !defined(__clang__)
      template <typename... Us>
      static decltype(auto) make_storage(Us... args) {
        return [args...](auto&& func) mutable -> decltype(auto) {
          return std::forward<decltype(func)>(func)(args...);
        };
      }
      using storage_type =
          decltype(make_storage(std::declval<move_t<Ts>>()...));
#else
      template <typename... Us>
      static decltype(auto) make_storage(Us&&... args) {
        return [](move_t<Ts>... mvs) {
          return [](move_t<Ts>... mvs) {
            return [mvs...](auto&& func) mutable -> decltype(auto) {
              return std::forward<decltype(func)>(func)(mvs...);
            };
          }(mvs...);
        }(std::forward<Us>(args)...);
      }
      using storage_type = decltype(make_storage(std::declval<Ts>()...));
#endif
      mutable storage_type storage;

      struct explicit_ctor_tag {};
      struct implicit_ctor_tag {};

      template <typename... Us>
      constexpr explicit tuple(implicit_ctor_tag, Us&&... args)
          : storage(make_storage(move_t<Ts>(std::forward<Us>(args))...)) {}

      template <typename... Us>
      constexpr explicit tuple(explicit_ctor_tag, Us&&... args)
          : tuple(Ts { std::forward<Us>(args) }...) {}

    public:
#if !defined(__clang__)
      template <typename... Us>
      constexpr explicit tuple(Us&&... args)
          : tuple(meta::if_t<meta::all<traits::is_convertible<Us, Ts>...>,
                             implicit_ctor_tag, explicit_ctor_tag>{},
                  std::forward<Us>(args)...) {}
#else
      template <typename... Us>
      constexpr explicit tuple(Us&&... args)
          : storage{make_storage(Ts(std::forward<Us>(args))...)} {}
#endif
      constexpr tuple() : tuple(Ts {}...) {}
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
      decltype(auto) apply(F&& f) const & {
        auto exec = [&f](move_t<Ts>&... xs) -> decltype(auto) {
#if !defined(__clang__)
          return std::forward<decltype(f)>(f)(static_cast<Ts const&>(xs)...);
#else
          return std::forward<decltype(f)>(f)(static_cast<Ts&>(xs)...);
#endif
        };
        return storage(exec);
      }

      template <typename F>
      decltype(auto) apply(F&& f) & {
        auto exec = [&f](move_t<Ts>&... xs) -> decltype(auto) {
          return std::forward<decltype(f)>(f)(
              static_cast<std::add_lvalue_reference_t<Ts>>(xs)...);
        };
        return storage(exec);
      }

      template <typename F>
      decltype(auto) apply(F&& f) && {
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
          return fused::tuple<std::remove_reference_t<Ts>...>(
              std::forward<Ts>(args)...);
        }
      };

      struct tie_ {
        template <typename... Ts>
        constexpr decltype(auto) operator()(Ts&... args) const {
          return fused::tuple<Ts&...>(args...);
        }
      };
    }

    constexpr detail::make_tuple_ make_tuple{};
    constexpr detail::tie_ tie{};

    template <std::size_t N, typename Tp>
    decltype(auto) get(Tp&& tp) {
#if defined(HAS_VARIABLE_TEMPLATES)
      return tp.apply(nth<N>);
#else
      return tp.apply(nth_v<N>::value);
#endif
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

    template <std::size_t I, typename Tp>
    using tuple_element_t = meta::eval<tuple_element<I, Tp>>;

    namespace detail {

      template <std::size_t...>
      struct tuple_recursion;

      template <>
      struct tuple_recursion<> {
        template <typename... Ts, typename... Us>
        static bool equal(tuple<Ts...> const&, tuple<Us...> const&) {
          return true;
        }
        template <typename... Ts>
        static bool less(tuple<Ts...> const&, tuple<Ts...> const&) {
          return false;
        }
      };

      template <std::size_t head, std::size_t... tail>
      struct tuple_recursion<head, tail...> {
        template <typename... Ts, typename... Us>
        static bool equal(tuple<Ts...> const& lhs, tuple<Us...> const& rhs) {
          return get<head>(lhs) == get<head>(rhs)
                     ? tuple_recursion<tail...>::equal(lhs, rhs)
                     : false;
        }
        template <typename... Ts>
        static bool less(tuple<Ts...> const& lhs, tuple<Ts...> const& rhs) {
          return get<head>(lhs) == get<head>(rhs)
                     ? tuple_recursion<tail...>::less(lhs, rhs)
                     : get<head>(lhs) < get<head>(rhs);
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
          meta::ignore{(fused::get<idx>(lhs) = std::move(fused::get<idx>(rhs)),
                        false)...};
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
      meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::copy_assign(
          *this, orig);
      return *this;
    }

    template <typename... Ts>
    tuple<Ts...>& tuple<Ts...>::operator=(tuple<Ts...>&& orig) {
      meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::move_assign(
          *this, std::move(orig));
      return *this;
    }

    template <typename... Ts>
    template <typename... Us>
    tuple<Ts...>& tuple<Ts...>::operator=(tuple<Us...> const& orig) {
      static_assert(sizeof...(Ts) <= sizeof...(Us), "");
      meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::copy_assign(
          *this, orig);
      return *this;
    }

    template <typename... Ts>
    template <typename... Us>
    bool tuple<Ts...>::operator==(tuple<Us...> const& rhs) const {
      return meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::equal(
          *this, rhs);
    }

    template <typename... Ts>
    bool tuple<Ts...>::operator<(tuple<Ts...> const& rhs) const {
      return meta::indexable_t<detail::tuple_internals, sizeof...(Ts)>::less(
          *this, rhs);
    }

    template <typename Tp>
    struct tuple_size;

    template <typename... Ts>
    struct tuple_size<tuple<Ts...>> : meta::number<sizeof...(Ts)> {};

    template <typename... Ts>
    struct tuple_size<const tuple<Ts...>> : tuple_size<tuple<Ts...>> {};
  }
}
}
