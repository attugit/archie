#ifndef ARCHIE_UTILS_FUSED_PLACEHOLDER_H_INCLUDED
#define ARCHIE_UTILS_FUSED_PLACEHOLDER_H_INCLUDED

#include <archie/utils/meta/at.h>

namespace archie {
namespace utils {
  namespace fused {

    template <std::size_t n>
    using placeholder = meta::at<n>;
  }
}
}

#endif
