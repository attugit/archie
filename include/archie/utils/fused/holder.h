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
      using value_type = std::decay_t<Tp>;
      using reference = value_type&;
      using const_reference = value_type const&;

      template <typename... Us>
      explicit holder(Us&&... u)
          : value(std::forward<Us>(u)...) {}

      template <typename Up,
                meta::requires_any<
                    traits::is_assignable<reference, std::decay_t<Up> const&>,
                    traits::is_assignable<reference, std::decay_t<Up>&&>>...>
      holder& operator=(Up&& u) {
        value = std::forward<Up>(u);
        return *this;
      }

      template <typename Up, meta::requires<traits::is_equality_comparable<
                                 const_reference, std::decay_t<Up> const&>>...>
      decltype(auto) operator==(Up const& u) const {
        return value == u;
      }

      template <typename Up, meta::requires<traits::is_less_than_comparable<
                                 const_reference, std::decay_t<Up> const&>>...>
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
