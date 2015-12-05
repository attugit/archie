#include <archie/utils/meta/indexable.h>
#include <catch.hpp>

namespace {
namespace meta = archie::utils::meta;

template <std::size_t... ids>
struct with_index {
  static const auto value = sizeof...(ids);
};

TEST_CASE("meta::indexable") {
  static_assert(meta::indexable_t<with_index, 0>::value == 0, "");
  static_assert(meta::indexable_t<with_index, 1>::value == 1, "");
  static_assert(meta::indexable_t<with_index, 2>::value == 2, "");
  static_assert(meta::indexable_t<with_index, 3>::value == 3, "");
}
}
