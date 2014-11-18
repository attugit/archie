#ifndef ARCHIE_UTILS_EAT_H_INCLUDED
#define ARCHIE_UTILS_EAT_H_INCLUDED

namespace archie {
namespace utils {

  template <typename>
  struct eat {
    template <typename Tp>
    eat(Tp&&) {}
  };
}
}

#endif
