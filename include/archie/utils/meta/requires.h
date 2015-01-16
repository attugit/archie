#ifndef ARCHIE_UTILS_META_REQUIRES_H_INCLUDED
#define ARCHIE_UTILS_META_REQUIRES_H_INCLUDED

#include <utility>
#include <archie/utils/meta/logic.h>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {

    using enabler = ignore;

    template <typename Condition>
    using requires = eval<std::enable_if<Condition::value, enabler>>;

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
