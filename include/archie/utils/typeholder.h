#ifndef ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED
#define ARCHIE_UTILS_TYPE_HOLDER_H_INCLUDED

namespace archie {
namespace utils {

template <typename T>
struct TypeHolder {
  using value_type = T;
  template <typename U>
  explicit TypeHolder(U u)
      : value(u) {}
  template <typename U>
  U& get() {
    return value;
  }
  template <typename U>
  U const& get() const {
    return value;
  }

private:
  value_type value;
};
}
}

#define ARCHIE_UTILS_GENERATE_TYPE_HOLDER(name, underlying)                    \
  struct name : archie::utils::TypeHolder<underlying> {                        \
    using BaseType = archie::utils::TypeHolder<underlying>;                    \
    using value_type = typename BaseType::value_type;                          \
    template <typename U>                                                      \
    explicit name(U u)                                                         \
        : BaseType(u) {}                                                       \
    using BaseType::get;                                                       \
  }

#endif
