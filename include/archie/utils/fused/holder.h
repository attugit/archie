#ifndef ARCHIE_UTILS_FUSED_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_FUSED_HOLDER_H_INCLUDED
#include <utility>
#include <type_traits>
#include <archie/utils/meta/requires.h>
#include <archie/utils/traits.h>

namespace archie {
namespace utils {
  namespace pure {
    template <typename Tp>
    using value_type_t = std::decay_t<Tp>;

    template <typename Tp>
    using reference_t = value_type_t<Tp>&;

    template <typename Tp>
    using const_reference_t = value_type_t<Tp> const&;

    template <typename Tp>
    using pointer_t = value_type_t<Tp>*;

    template <typename Tp>
    using const_pointer_t = value_type_t<Tp> const*;

    template <typename Tp>
    using rvalue_t = value_type_t<Tp>&&;
  }

  namespace fused {

    template <typename Tp>
    struct holder {
      using value_type = pure::value_type_t<Tp>;
      using reference = pure::reference_t<Tp>;
      using const_reference = pure::const_reference_t<Tp>;

      template <typename... Us>
      explicit holder(Us&&... u)
          : value(std::forward<Us>(u)...) {}

      template <
          typename Up,
          meta::requires_any<
              traits::is_assignable<reference, pure::const_reference_t<Up>>,
              traits::is_assignable<reference, pure::rvalue_t<Up>>>...>
      holder& operator=(Up&& u) {
        value = std::forward<Up>(u);
        return *this;
      }

      template <typename Up,
                meta::requires<traits::is_equality_comparable<
                    const_reference, pure::const_reference_t<Up>>>...>
      decltype(auto) operator==(Up const& u) const {
        return value == u;
      }

      template <typename Up,
                meta::requires<traits::is_less_than_comparable<
                    const_reference, pure::const_reference_t<Up>>>...>
      decltype(auto) operator<(Up const& u) const {
        return value < u;
      }

      operator reference() { return get(); }
      operator const_reference() const { return get(); }

      reference get() { return value; }
      const_reference get() const { return value; }

    private:
      value_type value;
    };
  }
}
}

#define GENERATE_HOLDER(n, t)                                                  \
  struct n : archie::utils::fused::holder<t> {                                 \
    using type = archie::utils::fused::holder<t>;                              \
    using type::type;                                                          \
  }
#endif
