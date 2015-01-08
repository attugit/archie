#ifndef ARCHIE_UTILS_META_LOGIC_H_INCLUDED
#define ARCHIE_UTILS_META_LOGIC_H_INCLUDED

#include <type_traits>
#include <archie/utils/meta/boolean.h>

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
      using type =
          decltype(test(typename std::conditional<Xs, int*, int>::type {}...));
    };

    template <bool... Xs>
    using conjunction_t = typename conjunction<Xs...>::type;

    template <bool... Xs>
    struct disjunction {
    private:
      static boolean<false> test();
      template <typename... Ts>
      static boolean<false> test(Ts*...);
      template <typename... Ts>
      static boolean<true> test(Ts...);

    public:
      using type =
          decltype(test(typename std::conditional<Xs, int, int*>::type {}...));
    };

    template <bool... Xs>
    using disjunction_t = typename disjunction<Xs...>::type;

    template <typename... Tp>
    using all = conjunction_t<Tp::value...>;

    template <typename... Tp>
    using any = disjunction_t<Tp::value...>;
  }
}
}

#endif
