#ifndef ARCHIE_UTILS_META_REQUIRES_H_INCLUDED
#define ARCHIE_UTILS_META_REQUIRES_H_INCLUDED

#include <utility>
#include <type_traits>
#include <archie/utils/meta/logic.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      struct enabler final : std::true_type {};
    }

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
