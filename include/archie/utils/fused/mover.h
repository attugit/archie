#ifndef ARCHIE_UTILS_FUSED_MOVER_H_INCLUDED
#define ARCHIE_UTILS_FUSED_MOVER_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {
  namespace fused {
    template <typename Tp>
    struct mover {
      using value_type = Tp;
      using reference = Tp&;
      using const_reference = Tp const&;
      mover(const_reference v) : value(v) {}
      mover(value_type&& v) : value(std::move(v)) {}
      mover(mover&&) = default;
      mover(mover const& other) = default;
      mover(mover& other) : value(std::move(other.value)) {}
      operator const_reference() const { return value; }
      operator reference() { return value; }
      decltype(auto) operator*() const { return value.operator*(); }
      decltype(auto) operator*() { return value.operator*(); }
      decltype(auto) operator -> () const { return value.operator->(); }
      decltype(auto) operator -> () { return value.operator->(); }
      value_type value;
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
