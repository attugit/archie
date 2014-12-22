#ifndef ARCHIE_UTILS_META_EAT_H_INCLUDED
#define ARCHIE_UTILS_META_EAT_H_INCLUDED

namespace archie {
namespace utils {
  namespace meta {

    struct ignore {
      ignore(...) {}
    };

    template <typename>
    using eat = ignore;
  }
}
}

#endif
