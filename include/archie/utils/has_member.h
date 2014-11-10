#ifndef ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED
#define ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED

#include <utility>
#include <archie/utils/voider.h>

namespace archie {
  namespace utils {

  template <template <typename> class, typename, typename = void>
  struct HasMember : std::false_type {};

  template <template <typename> class Member, typename Tp>
  struct HasMember<Member, Tp, Voider<Member<Tp>>> : std::true_type {};

  }
}

#endif
