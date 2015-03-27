#pragma once

#include <utility>
#include <archie/utils/fused/placeholder.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
    DECL_VARTEMPL(nth, placeholder, std::size_t);
  }
}
}
