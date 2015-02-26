#pragma once

#include <type_traits>
#include <archie/utils/meta/eval.h>
#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {

    template <bool... Xs>
    struct conjunction {
    private:
      static boolean<true> test();
      template <typename... Ts>
      static boolean<true> test(Ts*...);
      template <typename... Ts>
      static boolean<false> test(Ts...);

    public:
      using type = decltype(test(eval<std::conditional<Xs, int*, int>> {}...));
    };

    template <bool... Xs>
    using conjunction_t = eval<conjunction<Xs...>>;

    template <bool... Xs>
    struct disjunction {
    private:
      static boolean<false> test();
      template <typename... Ts>
      static boolean<false> test(Ts*...);
      template <typename... Ts>
      static boolean<true> test(Ts...);

    public:
      using type = decltype(test(eval<std::conditional<Xs, int, int*>> {}...));
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
}
