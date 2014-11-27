#ifndef ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED
#define ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED

#include <utility>
#include <archie/utils/meta/voider.h>

namespace archie {
namespace utils {
  namespace meta {

  template <template <typename> class, typename, typename = meta::void_t<>>
  struct has_member : std::false_type {};

  template <template <typename> class Member, typename Tp>
  struct has_member<Member, Tp, meta::void_t<Member<Tp>>> : std::true_type {};
  }
}
}

#endif
