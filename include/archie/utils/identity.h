#ifndef ARCHIE_UTILS_META_IDENTITY_H_INCLUDED
#define ARCHIE_UTILS_META_IDENTITY_H_INCLUDED

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    struct identity {
      using type = Tp;
    };

    template <typename Tp>
    using identity_t = typename identity<Tp>::type;
  }
}
}

#endif
