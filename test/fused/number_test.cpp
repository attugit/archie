#include <archie/number.hpp>

namespace
{
  namespace fused = archie::fused;
  static_assert(0 == fused::number<0>);
  static_assert(1 == fused::number<1>);
  static_assert(2 == fused::number<2>);
}
