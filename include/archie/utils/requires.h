#ifndef ARCHIE_UTILS_REQUIRES_H_INCLUDED
#define ARCHIE_UTILS_REQUIRES_H_INCLUDED

#include <utility>
#include <type_traits>

namespace archie {
namespace utils {
  template <bool B>
  using Boolean = std::integral_constant<bool, B>;

  template <typename... Conditions>
  using And = std::__and_<Conditions...>;

  template <typename... Conditions>
  using All = And<Conditions...>;

  template <typename... Conditions>
  using Or = std::__or_<Conditions...>;

  template <typename... Conditions>
  using Any = Or<Conditions...>;

  namespace detail {
    template <typename Condition>
    struct Not {
      using type = Boolean<!Condition::value>;
    };

    template <typename... Conditions>
    struct None {
      using type = Boolean<detail::Not<Any<Conditions...>>::type::value>;
    };

    struct enabler {};
  }

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
