#ifndef ARCHIE_UTILS_META_IGNORE_H_INCLUDED
#define ARCHIE_UTILS_META_IGNORE_H_INCLUDED

namespace archie {
namespace utils {
  namespace meta {

    struct ignore final {
      template <typename... Ts>
      ignore(Ts&&...) {}
    };

    template <typename>
    using eat = ignore;
  }
}
}

#endif
