#ifndef ARCHIE_UTILS_INHERIT_ALL_H_INCLUDED
#define ARCHIE_UTILS_INHERIT_ALL_H_INCLUDED

namespace archie {
namespace utils {

  template <typename... Ts>
  struct InheritAll : Ts... {};
}
}

#endif
