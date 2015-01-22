#pragma once

#include <archie/utils/meta/type_holder.h>
#include <utility>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp>
    struct type_tag : meta::type_holder<Tp> {
      template <typename... Us>
      Tp construct(Us&&... us) const {
        return Tp{std::forward<Us>(us)...};
      }
    };

    struct make_tag {
      template <typename Tp>
      decltype(auto) operator()(Tp&&) const noexcept {
        return type_tag<std::remove_reference_t<Tp>>{};
      }
    };
  }
}
}
