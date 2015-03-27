#pragma once

#if defined(__clang__) && ((__clang_major__ > 3) || ((__clang_major__ == 3) && __clang_minor__ > 3))
#define HAS_VARIABLE_TEMPLATES
#elif defined(__GNUC__) && (__GNUC__ > 4)
#define HAS_VARIABLE_TEMPLATES
#else
#undef HAS_VARIABLE_TEMPLATES
#endif

#if !defined(HAS_VARIABLE_TEMPLATES)
namespace archie {
namespace utils {
  namespace meta {
    template <typename Tp>
    struct variable_template {
      static constexpr decltype(Tp{}) const value = Tp{};
    };
    template <typename Tp>
    constexpr decltype(Tp{}) const variable_template<Tp>::value;
  }
}
}

#define DECL_VARTEMPL(name, type, param)                                                           \
  template <param T>                                                                               \
  struct name : archie::utils::meta::variable_template<type<T>> {}

#define DECL_VARIADIC_VARTEMPL(name, type, param)                                                  \
  template <param T>                                                                               \
  struct name : archie::utils::meta::variable_template<type<T...>> {}

#define VARTEMPL(name, ...) name<__VA_ARGS__>::value

#else

#define DECL_VARTEMPL(name, type, param)                                                           \
  template <param T>                                                                               \
  constexpr auto const name = type<T> {}

#define DECL_VARIADIC_VARTEMPL(name, type, param)                                                  \
  template <param T>                                                                               \
  constexpr auto const name = type<T...> {}

#define VARTEMPL(name, ...) name<__VA_ARGS__>
#endif
