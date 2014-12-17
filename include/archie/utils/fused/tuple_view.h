#ifndef ARCHIE_UTILS_FUSED_TUPLE_VIEW_H_INCLUDED
#define ARCHIE_UTILS_FUSED_TUPLE_VIEW_H_INCLUDED

#include <archie/utils/fused/tuple.h>
#include <functional>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp>
    struct value_view : std::reference_wrapper<Tp> {
      using base_t = std::reference_wrapper<Tp>;
      using base_t::base_t;
      template <typename Up>
      value_view& operator=(Up&& u) {
        this->get() = u;
        return *this;
      }
    };

    template <typename... Ts>
    using tuple_view = tuple<value_view<Ts>...>;

    template <typename... Ts>
    decltype(auto) make_view(Ts&... args) {
      return tuple<value_view<Ts>...>(args...);
    }

    template <std::size_t... idx, typename... Ts>
    decltype(auto) select(tuple<Ts...>& t) {
      return make_view(get<idx>(t)...);
    }
  }
}
}

#endif