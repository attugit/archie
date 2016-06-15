#include <archie/meta.hpp>
#include <gtest/gtest.h>

namespace
{
  namespace fused = archie::fused;
  static_assert(fused::type_list<> == fused::type_list<>);
  static_assert(fused::type_list<void> != fused::type_list<>);
  static_assert(fused::type_list<void> == fused::type_list<void>);
}
