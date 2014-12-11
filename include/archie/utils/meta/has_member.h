#ifndef ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED
#define ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED

#include <utility>
#include <archie/utils/meta/voider.h>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {

    template <template <typename...> class, typename, typename = meta::void_t<>>
    struct has_member : std::false_type {};

    template <template <typename...> class Member, typename... Ts>
    struct has_member<Member, type_list<Ts...>, meta::void_t<Member<Ts...>>>
        : std::true_type {};
  }
}
}

#endif
