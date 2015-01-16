#ifndef ARCHIE_UTILS_FUSED_TUPLE_H_INCLUDED
#define ARCHIE_UTILS_FUSED_TUPLE_H_INCLUDED

#include <archie/utils/meta/index_of.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/traits.h>
#include <archie/utils/fused/nth.h>
#include <archie/utils/fused/mover.h>
#include <utility>
#include <array>

namespace archie {
namespace utils {
  namespace fused {

    template <typename... Ts>
    struct tuple {
    private:
      template <typename... Us>
      static decltype(auto) make_storage(Us... args) {
        return [args...](auto&& func) mutable -> decltype(auto) {
          return std::forward<decltype(func)>(func)(static_cast<Ts&>(args)...);
        };
      }
      using storage_type =
          decltype(make_storage(std::declval<move_t<Ts>>()...));
      mutable storage_type storage;

    public:
      template <typename... Us,
                meta::requires_all<traits::is_convertible<Us, Ts>...>...>
      constexpr explicit tuple(Us&&... args)
          : storage(make_storage<move_t<Ts>...>(std::forward<Us>(args)...)) {}

      template <typename... Us, meta::requires_any<meta::opposite_t<
                                    traits::is_convertible<Us, Ts>>...>...>
      constexpr explicit tuple(Us&&... args)
          : tuple(Ts { std::forward<Us>(args) }...) {}

      constexpr tuple() : tuple(Ts {}...) {}

      tuple(tuple&&) = default;
      tuple(tuple const&) = default;
      tuple& operator=(tuple const&);
      tuple& operator=(tuple&&);

      template <typename... Us>
      tuple& operator=(tuple<Us...> const&);

      bool operator==(tuple const&) const;
      bool operator<(tuple const&) const;

      bool operator!=(tuple const& rhs) const { return !(*this == rhs); }
      bool operator>(tuple const& rhs) const { return rhs < *this; }
      bool operator>=(tuple const& rhs) const { return !(*this < rhs); }
      bool operator<=(tuple const& rhs) const { return !(*this > rhs); }

      constexpr std::size_t size() noexcept { return sizeof...(Ts); }

      template <typename F>
      decltype(auto) apply(F&& f) const {
        return storage(std::forward<F>(f));
      }
    };

    template <typename... Ts>
    decltype(auto) make_tuple(Ts&&... args) {
      return tuple<std::remove_reference_t<Ts>...>(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    decltype(auto) tie(Ts&... args) {
      return tuple<Ts&...>(args...);
    }

    template <std::size_t N, typename Tp>
    decltype(auto) get(Tp&& tp) {
      auto f = [](auto&&... args) -> decltype(auto) {
        return nth<N>(std::forward<decltype(args)>(args)...);
      };
      return tp.apply(f);
    }

    template <typename Tp, typename... Us>
    decltype(auto) get(tuple<Us...>& u) {
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
        template <typename... Ts>
        static bool equal(tuple<Ts...> const&, tuple<Ts...> const&) {
          return true;
        }
        template <typename... Ts>
        static bool less(tuple<Ts...> const&, tuple<Ts...> const&) {
          return false;
        }
      };

      template <std::size_t head, std::size_t... tail>
      struct tuple_recursion<head, tail...> {
        template <typename... Ts>
        static bool equal(tuple<Ts...> const& lhs, tuple<Ts...> const& rhs) {
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

      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct tuple_internals;

      template <std::size_t n, std::size_t... idx>
      struct tuple_internals<n, std::index_sequence<idx...>> {
        template <typename... Ts, typename... Us>
        static void copy_assign(tuple<Ts...>& lhs, tuple<Us...> const& rhs) {
          meta::ignore{(fused::get<idx>(lhs) = fused::get<idx>(rhs), false)...};
        }
        template <typename... Ts, typename... Us>
        static void move_assign(tuple<Ts...>& lhs, tuple<Us...>&& rhs) {
          meta::ignore{(fused::get<idx>(lhs) = std::move(fused::get<idx>(rhs)),
                        false)...};
        }
        template <typename... Ts>
        static bool equal(tuple<Ts...> const& lhs, tuple<Ts...> const& rhs) {
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
      detail::tuple_internals<sizeof...(Ts)>::copy_assign(*this, orig);
      return *this;
    }

    template <typename... Ts>
    tuple<Ts...>& tuple<Ts...>::operator=(tuple<Ts...>&& orig) {
      detail::tuple_internals<sizeof...(Ts)>::move_assign(*this,
                                                          std::move(orig));
      return *this;
    }

    template <typename... Ts>
    template <typename... Us>
    tuple<Ts...>& tuple<Ts...>::operator=(tuple<Us...> const& orig) {
      static_assert(sizeof...(Ts) <= sizeof...(Us), "");
      detail::tuple_internals<sizeof...(Ts)>::copy_assign(*this, orig);
      return *this;
    }

    template <typename... Ts>
    bool tuple<Ts...>::operator==(tuple<Ts...> const& rhs) const {
      return detail::tuple_internals<sizeof...(Ts)>::equal(*this, rhs);
    }

    template <typename... Ts>
    bool tuple<Ts...>::operator<(tuple<Ts...> const& rhs) const {
      return detail::tuple_internals<sizeof...(Ts)>::less(*this, rhs);
    }

    template <typename Tp>
    struct tuple_size;

    template <typename... Ts>
    struct tuple_size<tuple<Ts...>> : meta::number<sizeof...(Ts)> {};
  }
}
}

#endif
