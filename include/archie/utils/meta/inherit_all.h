#ifndef ARCHIE_UTILS_META_INHERIT_ALL_H_INCLUDED
#define ARCHIE_UTILS_META_INHERIT_ALL_H_INCLUDED

namespace archie {
namespace utils {
  namespace meta {

    template <typename... Ts>
    struct inherit_all : Ts... {};
  }
}
}

#endif
