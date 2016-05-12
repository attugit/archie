#include <archie/traits/is_fused_tuple.hpp>
#include <archie/fused/tuple.hpp>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;
namespace fused = archie::fused;
namespace traits = archie::traits;

TEST_CASE("is_fused_tuple")
{
  static_assert(traits::is_fused_tuple<fused::tuple<int>>::value, "");
  static_assert(!traits::is_fused_tuple<int>::value, "");
  static_assert(!traits::is_fused_tuple<meta::type_list<int>>::value, "");
  static_assert(!traits::is_fused_tuple<meta::type_list<fused::tuple<int>>>::value, "");
  static_assert(traits::is_fused_tuple<fused::tuple<meta::type_list<int>>>::value, "");
}
}
