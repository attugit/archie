#ifndef ARCHIE_UTILS_META_REQUIRES_H_INCLUDED
#define ARCHIE_UTILS_META_REQUIRES_H_INCLUDED

#include <utility>
#include <type_traits>

namespace archie {
namespace utils {
  namespace meta {
    template <bool B>
    using boolean = std::integral_constant<bool, B>;

    template <typename... Conditions>
    using all = std::__and_<Conditions...>;

    template <typename... Conditions>
    using any = std::__or_<Conditions...>;

    namespace detail {
      template <typename Condition>
      struct __not_ {
        using type = boolean<!Condition::value>;
      };

      template <typename... Conditions>
      struct none {
        using type = boolean<detail::__not_<any<Conditions...>>::type::value>;
      };

      struct enabler final : std::true_type {};
    }

    template <typename... Conditions>
    struct none : public detail::none<Conditions...>::type {};

    template <typename Condition>
    using requires =
        typename std::enable_if<Condition::value, detail::enabler>::type;

    template <typename... Conditions>
    using requires_all = requires<all<Conditions...>>;

    template <typename... Conditions>
    using requires_any = requires<any<Conditions...>>;

    template <typename... Conditions>
    using requires_none = requires<none<Conditions...>>;
  }
}
}
#endif
