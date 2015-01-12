#ifndef ARCHIE_UTILS_TRAITS_IS_CONVERTIBLE_H_INCLUDED
#define ARCHIE_UTILS_TRAITS_IS_CONVERTIBLE_H_INCLUDED

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      template <typename Tp>
      struct convert_helper {
        static void implicit(Tp);
      };

      template <typename Tp, typename Up>
      using inspect_convertible =
          decltype(convert_helper<Up>::implicit(std::declval<Tp>()));
    }
    template <typename Tp, typename Up>
    using is_convertible =
        meta::has_member<detail::inspect_convertible, meta::type_list<Tp, Up>>;
  }
}
}

#endif
