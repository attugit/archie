#pragma once

#include <utility>
#include <archie/meta.hpp>
#include <archie/traits.hpp>

namespace archie::fused
{
  namespace detail
  {
    template <typename Tp>
    struct move_capture {
      using value_type = std::decay_t<Tp>;
      using reference = value_type&;
      using const_reference = value_type const&;
      template <typename Up>
      move_capture(Up&& u) : value(std::forward<Up>(u))
      {
      }
      move_capture(move_capture&&) = default;
      move_capture(move_capture const&) = default;
      move_capture(move_capture& other) : value(std::forward<Tp>(other.value)) {}
      operator Tp&() { return value; }
      operator Tp const&() const { return value; }
      Tp value;
    };
  }

  template <typename Tp>
  using move_t =
      meta::if_t<meta::all<std::is_move_constructible<Tp>,
                           meta::any<std::is_reference<Tp>,
                                     meta::no<std::is_copy_constructible<Tp>>,
                                     meta::no<traits::is_trivially_copy_constructible<Tp>>>>,
                 detail::move_capture<Tp>,
                 Tp>;
}
