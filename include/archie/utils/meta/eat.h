#ifndef ARCHIE_UTILS_META_EAT_H_INCLUDED
#define ARCHIE_UTILS_META_EAT_H_INCLUDED

namespace archie {
namespace utils {
  namespace meta {

    template <typename>
    struct eat {
      eat(...) {}
    };

    using ignore = eat<void>;
  }
}
}

#endif
