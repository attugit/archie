#ifndef ARCHIE_UTILS_FUSED_TUPLE_H_INCLUDED
#define ARCHIE_UTILS_FUSED_TUPLE_H_INCLUDED

#include <archie/utils/meta/type_list.h>
#include <archie/utils/fused/nth.h>
#include <utility>

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
      struct copy_assign_tuple;

      template <std::size_t n, std::size_t... idx>
      struct copy_assign_tuple<n, std::index_sequence<idx...>> {
        template <typename... Ts>
        static void impl(tuple<Ts...>& lhs, tuple<Ts...> const& rhs) {
          using alias = int[];
          (void)alias{0, (fused::get<idx>(lhs) = fused::get<idx>(rhs), 0)...};
        }
      };
    }

    template <typename... Ts>
    tuple<Ts...>& tuple<Ts...>::operator=(tuple<Ts...> const& orig) {
      detail::copy_assign_tuple<sizeof...(Ts)>::impl(*this, orig);
      return *this;
    }
  }
}
}

#endif
