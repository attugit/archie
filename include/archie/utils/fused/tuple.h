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
      constexpr static decltype(auto) make_storage(Us const&... args) {
        return [args...](auto&& func) -> decltype(auto) {
          return std::forward<decltype(func)>(func)(args...);
        };
      }
      using storage_type = decltype(make_storage(std::declval<Ts>()...));
      storage_type storage;

    public:
      using type_list = meta::type_list<Ts...>;

      template <typename... Up>
      constexpr explicit tuple(Up&&... args)
          : storage(make_storage(std::forward<Up>(args)...)) {}

      constexpr std::size_t size() noexcept { return sizeof...(Ts); }

      template <typename F>
      decltype(auto) apply(F&& f) {
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
  }
}
}

#endif
