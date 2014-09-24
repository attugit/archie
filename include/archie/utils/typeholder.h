#ifndef ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED

#include <experimental/optional>

namespace archie {
namespace utils {

  template <typename Tp>
  using TypeHolder = std::experimental::optional<Tp>;
}
}

#define ARCHIE_UTILS_GENERATE_TYPE_HOLDER(name, underlying)                    \
  struct name : archie::utils::TypeHolder<underlying> {                        \
    using BaseType = archie::utils::TypeHolder<underlying>;                    \
    using value_type = typename BaseType::value_type;                          \
    using BaseType::BaseType;                                                  \
    template <typename U>                                                      \
    name(U&& u)                                                                \
        : BaseType(std::forward<U>(u)) {}                                      \
    using BaseType::operator*;                                                 \
    using BaseType::operator->;                                                \
    using BaseType::operator bool;                                             \
  }

#endif
