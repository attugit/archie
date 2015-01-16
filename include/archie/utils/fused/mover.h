#ifndef ARCHIE_UTILS_FUSED_MOVER_H_INCLUDED
#define ARCHIE_UTILS_FUSED_MOVER_H_INCLUDED

#include <utility>
#include <archie/utils/meta/functional.h>
#include <archie/utils/meta/logic.h>

namespace archie {
namespace utils {
  namespace fused {
    template <typename Tp>
    struct mover {
      using value_type = std::decay_t<Tp>;
      using reference = value_type&;
      template <typename Up>
      mover(Up&& u)
          : value(std::forward<Up>(u)) {}
      mover(mover&&) = default;
      mover(mover const&) = default;
      mover(mover& other) : value(std::forward<Tp>(other.value)) {}
      operator reference() { return value; }
      Tp value;
    };

    template <typename Tp>
    using move_t = meta::if_t<
        meta::all<
            std::is_move_constructible<Tp>,
            meta::any<std::is_reference<Tp>,
                      meta::opposite_t<std::is_copy_constructible<Tp>>,
                      meta::opposite_t<std::has_trivial_copy_constructor<Tp>>>>,
        mover<Tp>, Tp>;
  }
}
}

#endif
