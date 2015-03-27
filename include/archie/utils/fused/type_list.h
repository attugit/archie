#pragma once

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
    DECL_VARIADIC_VARTEMPL(type_list, meta::type_list, typename...);
  }
}
}
