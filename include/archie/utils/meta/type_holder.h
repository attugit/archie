#ifndef ARCHIE_UTILS_META_TYPE_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_META_TYPE_HOLDER_H_INCLUDED

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    struct type_holder {
      using type = Tp;
    };
  }
}
}

#endif
