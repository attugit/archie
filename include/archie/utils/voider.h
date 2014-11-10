#ifndef ARCHIE_UTILS_VOIDER_H_INCLUDED
#define ARCHIE_UTILS_VOIDER_H_INCLUDED

namespace archie {
namespace utils {

  namespace detail {
    template <typename Tp, typename...>
    struct VariadicWrapper {
      using type = Tp;
    };
  }

  template <typename... Ts>
  using Voider = typename detail::VariadicWrapper<void, Ts...>::type;
}
}

#endif
