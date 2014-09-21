#ifndef ARCHIE_UTILS_REQUIRES_H_INCLUDED
#define ARCHIE_UTILS_REQUIRES_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {
  template <bool B>
  using Boolean = std::integral_constant<bool, B>;

  namespace detail {

    template <typename... Conditions>
    struct All;

    template <typename Head, typename... Tail>
    struct All<Head, Tail...> {
      using type = Boolean<Head::value && detail::All<Tail...>::type::value>;
    };

    template <typename Head>
    struct All<Head> {
      using type = Boolean<Head::value>;
    };

    template <typename... Conditions>
    struct Any;

    template <typename Head, typename... Tail>
    struct Any<Head, Tail...> {
      using type = Boolean<Head::value || detail::Any<Tail...>::type::value>;
    };

    template <typename Head>
    struct Any<Head> {
      using type = Boolean<Head::value>;
    };

    template <typename Condition>
    struct Not {
      using type = Boolean<!Condition::value>;
    };

    template <typename... Conditions>
    struct None {
      using type = Boolean<
          detail::Not<typename detail::Any<Conditions...>::type>::type::value>;
    };

    struct enabler {};
  }

  template <typename... Conditions>
  struct All : public detail::All<Conditions...>::type {};

  template <typename... Conditions>
  struct Any : public detail::Any<Conditions...>::type {};

  template <typename Condition>
  struct Not : public detail::Not<Condition>::type {};

  template <typename... Conditions>
  struct None : public detail::None<Conditions...>::type {};

  template <typename Condition>
  using Requires =
      typename std::enable_if<Condition::value, detail::enabler>::type;

  template <typename... Conditions>
  using RequiresAll = Requires<All<Conditions...>>;

  template <typename... Conditions>
  using RequiresAny = Requires<Any<Conditions...>>;

  template <typename... Conditions>
  using RequiresNone = Requires<None<Conditions...>>;
}
}

#endif
