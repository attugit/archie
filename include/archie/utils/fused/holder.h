#ifndef ARCHIE_UTILS_FUSED_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_FUSED_HOLDER_H_INCLUDED
#include <utility>
#include <type_traits>

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

      holder& operator=(const_reference v) {
        value = v;
        return *this;
      }
      holder& operator=(value_type&& v) {
        value = std::move(v);
        return *this;
      }

      operator reference() { return value; }
      operator const_reference() const { return value; }

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
