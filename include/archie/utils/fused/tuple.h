#ifndef ARCHIE_UTILS_FUSED_TUPLE_H_INCLUDED
#define ARCHIE_UTILS_FUSED_TUPLE_H_INCLUDED

#include <archie/utils/meta/type_list.h>
#include <archie/utils/fused/nth.h>
#include <utility>
#include <array>

namespace archie {
namespace utils {
  namespace fused {

    template <typename... Ts>
    struct tuple {
    private:
      template <typename... Us>
      static decltype(auto) make_storage(Us&&... args) {
        return [args...](auto&& func) mutable -> decltype(auto) {
          return std::forward<decltype(func)>(func)(args...);
        };
      }
      using storage_type = decltype(make_storage(std::declval<Ts>()...));
      mutable storage_type storage;

    public:
      using type_list = meta::type_list<Ts...>;

      template <typename... Up>
      constexpr explicit tuple(Up&&... args)
          : storage(make_storage(std::forward<Up>(args)...)) {}

      constexpr tuple() : tuple(Ts {}...) {}

      tuple(tuple&&) = default;
      tuple(tuple const&) = default;
      tuple& operator=(tuple const&);
      tuple& operator=(tuple&&) = default;

      bool operator==(tuple const&) const;
      bool operator!=(tuple const& rhs) const { return !(*this == rhs); }

      constexpr std::size_t size() noexcept { return sizeof...(Ts); }

      template <typename F>
      decltype(auto) apply(F&& f) const {
        return storage(std::forward<F>(f));
      }
    };

    template <typename... Ts>
    decltype(auto) make_tuple(Ts&&... args) {
      return tuple<Ts...>(std::forward<Ts>(args)...);
    }

    template <std::size_t N, typename Tp>
    decltype(auto) get(Tp&& tp) {
      auto f = [](auto&&... args) -> decltype(auto) {
        return nth<N>(std::forward<decltype(args)>(args)...);
      };
      return tp.apply(f);
    }

    namespace detail {
      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct tuple_internals;

      template <std::size_t n, std::size_t... idx>
      struct tuple_internals<n, std::index_sequence<idx...>> {
        using alias = std::array<bool, sizeof...(idx)+1>;
        template <typename... Ts>
        static void copy_assign(tuple<Ts...>& lhs, tuple<Ts...> const& rhs) {
          (void)alias{false,
                      (fused::get<idx>(lhs) = fused::get<idx>(rhs), false)...};
        }
        template <typename... Ts>
        static bool equal(tuple<Ts...> const& lhs, tuple<Ts...> const& rhs) {
          return alias{true, (get<idx>(lhs) == get<idx>(rhs))...} ==
                 alias{true, ((void)idx, true)...};
        }
      };
    }

    template <typename... Ts>
    tuple<Ts...>& tuple<Ts...>::operator=(tuple<Ts...> const& orig) {
      detail::tuple_internals<sizeof...(Ts)>::copy_assign(*this, orig);
      return *this;
    }

    template <typename... Ts>
    bool tuple<Ts...>::operator==(tuple<Ts...> const& rhs) const {
      return detail::tuple_internals<sizeof...(Ts)>::equal(*this, rhs);
    }
  }
}
}

#endif
