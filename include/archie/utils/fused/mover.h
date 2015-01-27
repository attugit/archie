#pragma once

#include <utility>
#include <functional>
#include <archie/utils/meta/functional.h>
#include <archie/utils/meta/logic.h>
#include <archie/utils/traits.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <typename Tp>
      struct move_capture {
        using value_type = std::decay_t<Tp>;
        using reference = value_type&;
        template <typename Up>
        move_capture(Up&& u)
            : value(std::move(u)) {}
        move_capture(move_capture&&) = default;
        move_capture(move_capture const&) = default;
        move_capture(move_capture& other) : value(std::move(other.value)) {}
        operator reference() { return value; }
        Tp value;
      };
    }
    template <typename Tp>
    using move_t = meta::if_t<
        meta::all<std::is_move_constructible<Tp>,
                  meta::any<meta::opposite_t<std::is_copy_constructible<Tp>>,
                            meta::opposite_t<
                                traits::is_trivially_copy_constructible<Tp>>>>,
        detail::move_capture<Tp>,
        meta::if_t<std::is_reference<Tp>,
                   std::reference_wrapper<std::remove_reference_t<Tp>>, Tp>>;
  }
}
}
