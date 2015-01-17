#pragma once

#include <type_traits>

namespace archie {
namespace utils {
  namespace traits {
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
  }
}
}
