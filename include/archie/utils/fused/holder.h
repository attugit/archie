#ifndef ARCHIE_UTILS_FUSED_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_FUSED_HOLDER_H_INCLUDED
#include <utility>
#include <type_traits>
#include <archie/utils/meta/requires.h>
#include <archie/utils/traits.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp>
    struct holder {
      using value_type = traits::pure::value_type_t<Tp>;
      using reference = traits::pure::reference_t<Tp>;
      using const_reference = traits::pure::const_reference_t<Tp>;
      using pointer = traits::pure::pointer_t<Tp>;
      using const_pointer = traits::pure::const_pointer_t<Tp>;

      template <typename... Us>
      explicit holder(Us&&... u)
          : value(std::forward<Us>(u)...) {}

      template <
          typename Up,
          meta::requires_any<
              traits::is_assignable<reference,
                                    traits::pure::const_reference_t<Up>>,
              traits::is_assignable<reference, traits::pure::rvalue_t<Up>>>...>
      holder& operator=(Up&& u) {
        value = std::forward<Up>(u);
        return *this;
      }

      template <typename Up,
                meta::requires<traits::is_equality_comparable<
                    const_reference, traits::pure::const_reference_t<Up>>>...>
      friend bool operator==(holder<Tp> const& h, Up const& u) {
        return h.value == u;
      }

      template <typename Up,
                meta::requires<traits::is_equality_comparable<
                    const_reference, traits::pure::const_reference_t<Up>>>...>
      friend bool operator!=(holder<Tp> const& h, Up const& u) {
        return !(h.value == u);
      }

      template <typename Up,
                meta::requires<traits::is_less_than_comparable<
                    const_reference, traits::pure::const_reference_t<Up>>>...>
      friend bool operator<(holder<Tp> const& h, Up const& u) {
        return h.value < u;
      }

      template <typename Up,
                meta::requires<traits::is_less_than_comparable<
                    const_reference, traits::pure::const_reference_t<Up>>>...>
      friend bool operator>=(holder<Tp> const& h, Up const& u) {
        return !(h.value < u);
      }

      template <typename Up,
                meta::requires<traits::is_less_than_comparable<
                    traits::pure::const_reference_t<Up>, const_reference>>...>
      friend bool operator>(holder<Tp> const& h, Up const& u) {
        return u < h.value;
      }

      template <typename Up,
                meta::requires<traits::is_less_than_comparable<
                    traits::pure::const_reference_t<Up>, const_reference>>...>
      friend bool operator<=(holder<Tp> const& h, Up const& u) {
        return !(u < h.value);
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
