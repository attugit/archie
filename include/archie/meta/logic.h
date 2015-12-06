#pragma once

#include <type_traits>
#include <archie/meta/eval.h>
#include <archie/fused/boolean.h>
#include <archie/meta/returns.h>

namespace archie {
namespace meta {
  template <bool... Xs>
  struct conjunction {
  private:
    static decltype(fused::True) test();
    template <typename... Ts>
    static decltype(fused::True) test(Ts*...);
    template <typename... Ts>
    static decltype(fused::False) test(Ts...);

  public:
    using type = decltype(test(eval<std::conditional<Xs, int*, int>>{}...));
  };

  template <bool... Xs>
  using conjunction_t = eval<conjunction<Xs...>>;

  template <bool... Xs>
  struct disjunction {
  private:
    static decltype(fused::False) test();
    template <typename... Ts>
    static decltype(fused::False) test(Ts*...);
    template <typename... Ts>
    static decltype(fused::True) test(Ts...);

  public:
    using type = decltype(test(eval<std::conditional<Xs, int, int*>>{}...));
  };

  template <bool... Xs>
  using disjunction_t = eval<disjunction<Xs...>>;

  template <bool B>
  using negation = boolean<!B>;

  template <bool B>
  using negation_t = eval<negation<B>>;

  template <typename Tp>
  using opposite = negation<Tp::value>;

  template <typename Tp>
  using opposite_t = negation_t<Tp::value>;

  template <typename... Ts>
  using all = conjunction_t<Ts::value...>;

  template <typename... Ts>
  using any = disjunction_t<Ts::value...>;

  template <typename... Ts>
  using none = opposite_t<any<Ts...>>;
}
}
