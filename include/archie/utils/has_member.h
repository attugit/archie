#ifndef ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED
#define ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED

#include <utility>
#include <archie/utils/voider.h>

namespace archie {
namespace utils {

  template <template <typename> class, typename, typename = void_t<>>
  struct HasMember : std::false_type {};

  template <template <typename> class Member, typename Tp>
  struct HasMember<Member, Tp, void_t<Member<Tp>>> : std::true_type {};
}
}

#endif
