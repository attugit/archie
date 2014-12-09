#ifndef ARCHIE_UTILS_FUSED_MOVER_H_INCLUDED
#define ARCHIE_UTILS_FUSED_MOVER_H_INCLUDED

#include <utility>

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
    using move_t = typename std::conditional<
        std::is_move_constructible<Tp>::value&&(
            !std::is_copy_constructible<Tp>::value ||
            !std::has_trivial_copy_constructor<Tp>::value),
        mover<Tp>, Tp>::type;
  }
}
}

#endif
